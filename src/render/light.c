/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leokubler <leokubler@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:40:34 by lkubler           #+#    #+#             */
/*   Updated: 2025/04/23 16:22:11 by leokubler        ###   ########.fr       */
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

double	compute_shadow(t_scene *scene, t_vec3 point, t_light light)
{
	const int	samples;
	int			unblocked;
	double		dist;
	double		t;
	bool		blocked;

	
}

t_color compute_lighting(t_scene *scene, t_hit hit)
{
	t_color final_color = color_scale(scene->ambient.color, scene->ambient.ratio);  // Ambient als Grundfarbe

	for (int i = 0; i < scene->light_count; i++)
	{
		t_light light = scene->lights[i];
		t_vec3 light_dir = vec_sub(light.position, hit.point);
		double light_dist = vec_length(light_dir);
		light_dir = vec_normalize(light_dir);

		// Schattenstrahl
		t_ray shadow_ray;
		shadow_ray.origin = vec_add(hit.point, vec_mul(light_dir, 1e-4));  // kleine Verschiebung nach außen
		shadow_ray.direction = light_dir;

		bool in_shadow = false;
		for (int j = 0; j < scene->object_count; j++)
		{
			t_hit shadow_hit;
			double t = scene->objects[j].hit(&scene->objects[j], shadow_ray, &shadow_hit);
			if (t > 0 && t < light_dist)
			{
				in_shadow = true;
				break;
			}
		}
		if (!in_shadow)
		{
			double diffuse = fmax(0.0, vec_skal(hit.normal, light_dir));
			t_color light_contrib = color_scale(hit.color, diffuse * light.brightness);
			light_contrib = color_mix(light_contrib, light.color, 0.2);
			final_color = color_add(final_color, light_contrib);
		}
	}
	return color_clamp(final_color);
}
