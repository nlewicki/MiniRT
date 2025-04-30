/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/30 11:51:48 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/miniRT.h"

t_color color_scale(t_color c, double factor)
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
	double u = (double)rand() / RAND_MAX;
	double v = (double)rand() / RAND_MAX;
	double theta = 2.0 * M_PI * u;
	double phi = acos(2.0 * v - 1.0);
	double x = radius * sin(phi) * cos(theta);
	double y = radius * sin(phi) * sin(theta);
	double z = radius * cos(phi);
	return ((t_vec3){center.x + x, center.y + y, center.z + z});
}

static double	compute_shadow_factor(t_miniRT *mini, t_vec3 point, t_light light)
{
	int			unblocked;
	double		dist;
	double		t;
	bool		blocked;

	unblocked = 0;
	for (int i = 0; i < mini->samples; i++)
	{
		blocked = false;
		t_vec3 samples_pos = random_points(light.position, 0.2);
		t_vec3 dir = vec_sub(samples_pos, point);
		dist = vec_length(dir);
		dir = vec_normalize(dir);
		t_ray shadow_ray;
		shadow_ray.origin = vec_add(point, vec_mul(dir, 1e-4));
		shadow_ray.direction = dir;
		for (int j = 0; j < mini->scene.object_count; j++)
		{
			t_hit hit;
			t = mini->scene.objects[j].hit(&mini->scene.objects[j], shadow_ray, &hit);
			if (t > 0 && t < dist)
			{
				blocked = true;
				break;
			}
		}
		if (!blocked)
			unblocked ++;
	}
	return ((double)unblocked / (double)mini->samples); // Minimum brightness factor
}

t_color compute_lighting(t_miniRT *mini, t_hit hit)
{
	t_color final_color = color_scale(mini->scene.ambient.color, mini->scene.ambient.ratio);  // Ambient als Grundfarbe

	for (int i = 0; i < mini->scene.light_count; i++)
	{
		t_light light = mini->scene.lights[i];
		t_vec3 light_dir = vec_sub(light.position, hit.point);
		light_dir = vec_normalize(light_dir);

		// Schattenstrahl
		t_ray shadow_ray;
		shadow_ray.origin = vec_add(hit.point, vec_mul(light_dir, 1e-4));  // kleine Verschiebung nach außen
		shadow_ray.direction = light_dir;

		double shadow = compute_shadow_factor(mini, hit.point, light);
		shadow = pow(shadow, 0.7);
		if (shadow > 0.0)
		{
			// ---------- DIFFUSE ----------
			double diffuse = fmax(0.0, vec_skal(hit.normal, light_dir));
			t_color light_contrib = color_scale(hit.material.color, diffuse * light.brightness * shadow);

			// ---------- SPECULAR ----------
			t_vec3 view_dir = vec_normalize(vec_sub(mini->scene.camera.position, hit.point));
			t_vec3 reflect_dir = vec_reflect(vec_neg(light_dir), hit.normal);
			double spec = pow(fmax(vec_skal(reflect_dir, view_dir), 0.0), hit.material.shine);
			t_color specular_color = color_scale(light.color, hit.material.ks * spec * light.brightness * shadow);

			// Kombinieren
			t_color combined = color_add(light_contrib, specular_color);
			light_contrib = color_mix(combined, light.color, 0.2);  // kleinere Farbmischung für Lichtfarbe
			final_color = color_add(final_color, light_contrib);
		}
	}
	return color_clamp(final_color);
}

