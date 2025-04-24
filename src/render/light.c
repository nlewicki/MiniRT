/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:40:34 by lkubler           #+#    #+#             */
/*   Updated: 2025/04/24 11:56:49 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static t_color color_scale(t_color c, double factor)
{
	t_color result = {
		.r = c.r * factor,
		.g = c.g * factor,
		.b = c.b * factor,
		.a = c.a
	};
	return result;
}

//static t_color color_mult(t_color c1, t_color c2)
//{
//	t_color result = {
//		.r = (c1.r * c2.r) / 255,
//		.g = (c1.g * c2.g) / 255,
//		.b = (c1.b * c2.b) / 255,
//		.a = c1.a
//	};
//	return result;
//}

static t_color color_add(t_color c1, t_color c2)
{
	t_color result = {
		.r = c1.r + c2.r,
		.g = c1.g + c2.g,
		.b = c1.b + c2.b,
		.a = c1.a
	};
	return result;
}

static t_color color_clamp(t_color c)
{
	c.r = fmin(255, fmax(0, c.r));
	c.g = fmin(255, fmax(0, c.g));
	c.b = fmin(255, fmax(0, c.b));
	return c;
}

t_color color_mix(t_color a, t_color b, double factor)
{
	t_color result;

	result.r = a.r * (1 - factor) + b.r * factor;
	result.g = a.g * (1 - factor) + b.g * factor;
	result.b = a.b * (1 - factor) + b.b * factor;
	return result;
}

static t_vec3	random_points(t_vec3 center, double radius)
{
	double u = (double)rand() / RAND_MAX * 2.0 - 1.0;
	double v = (double)rand() / RAND_MAX * 2.0 - 1.0;
	double w = (double)rand() / RAND_MAX * 2.0 - 1.0;
	t_vec3 offset = {u * radius, v * radius, w * radius};
	return vec_add(center, offset);
}

static double	compute_shadow_factor(t_scene *scene, t_vec3 point, t_light light)
{
	const int base_samples = 8;
	int samples;
	double dist_to_light = vec_length(vec_sub(light.position, point));

	// Adaptive sampling based on distance
	if (dist_to_light < 5.0)
		samples = base_samples;
	else if (dist_to_light < 10.0)
		samples = base_samples / 2;
	else
		samples = base_samples / 4;

	if (samples < 2)
		samples = 2;

	int unblocked = 0;
	double radius = 0.2 * (dist_to_light / 10.0); // Adaptive light radius

	// Quick test for full shadow with center ray
	t_vec3 dir = vec_sub(light.position, point);
	double dist = vec_length(dir);
	dir = vec_normalize(dir);
	t_ray shadow_ray = {vec_add(point, vec_mul(dir, 1e-4)), dir};

	bool fully_shadowed = false;
	for (int j = 0; j < scene->object_count; j++)
	{
		t_hit hit;
		double t = scene->objects[j].hit(&scene->objects[j], shadow_ray, &hit);
		if (t > 0 && t < dist)
		{
			fully_shadowed = true;
			break;
		}
	}

	if (fully_shadowed)
		return 0.0;

	// Soft shadow sampling
	for (int i = 0; i < samples; i++)
	{
		t_vec3 samples_pos = random_points(light.position, radius);
		dir = vec_sub(samples_pos, point);
		dist = vec_length(dir);
		dir = vec_normalize(dir);
		shadow_ray.origin = vec_add(point, vec_mul(dir, 1e-4));
		shadow_ray.direction = dir;

		bool blocked = false;
		for (int j = 0; j < scene->object_count; j++)
		{
			t_hit hit;
			double t = scene->objects[j].hit(&scene->objects[j], shadow_ray, &hit);
			if (t > 0 && t < dist)
			{
				blocked = true;
				break;
			}
		}
		if (!blocked)
			unblocked++;
	}
	return ((double)unblocked / (double)samples);
}

t_color compute_lighting(t_scene *scene, t_hit hit)
{
	t_color final_color = color_scale(scene->ambient.color, scene->ambient.ratio);  // Ambient als Grundfarbe

	for (int i = 0; i < scene->light_count; i++)
	{
		t_light light = scene->lights[i];
		t_vec3 light_dir = vec_sub(light.position, hit.point);
		// double light_dist = vec_length(light_dir);
		light_dir = vec_normalize(light_dir);

		// Schattenstrahl
		t_ray shadow_ray;
		shadow_ray.origin = vec_add(hit.point, vec_mul(light_dir, 1e-4));  // kleine Verschiebung nach außen
		shadow_ray.direction = light_dir;

		double shadow = compute_shadow_factor(scene, hit.point, light);
		if (shadow > 0.0)
		{
			double diffuse = fmax(0.0, vec_skal(hit.normal, light_dir));
			t_color light_contrib = color_scale(hit.color, diffuse * light.brightness * shadow);
			light_contrib = color_mix(light_contrib, light.color, 0.2);
			final_color = color_add(final_color, light_contrib);
		}

	}
	return color_clamp(final_color);
}
