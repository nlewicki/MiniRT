/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:40:34 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/22 12:46:30 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	color_scale(t_color c, double factor)
{
	t_color	result;

	result.r = c.r * factor;
	result.g = c.g * factor;
	result.b = c.b * factor;
	result.a = c.a;
	return (result);
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

static t_color	color_add(t_color c1, t_color c2)
{
	t_color	result;

	result.r = c1.r + c2.r;
	result.g = c1.g + c2.g;
	result.b = c1.b + c2.b;
	result.a = c1.a;
	return (result);
}

t_color	color_mix(t_color a, t_color b, double factor)
{
	t_color	result;

	result.r = a.r * (1 - factor) + b.r * factor;
	result.g = a.g * (1 - factor) + b.g * factor;
	result.b = a.b * (1 - factor) + b.b * factor;
	return (result);
}

static t_vec3	random_points(t_vec3 center, double radius)
{
	double	u;
	double	v;
	double	theta;
	double	phi;
	double	x;
	double	y;
	double	z;
	t_vec3	point;

	u = (double)rand() / RAND_MAX;
	v = (double)rand() / RAND_MAX;
	theta = 2.0 * M_PI * u;
	phi = acos(2.0 * v - 1.0);
	x = radius * sin(phi) * cos(theta);
	y = radius * sin(phi) * sin(theta);
	z = radius * cos(phi);
	point.x = center.x + x;
	point.y = center.y + y;
	point.z = center.z + z;
	return (point);
}

static bool	is_shadow_blocked(t_miniRT *mini, t_ray shadow_ray,
	double dist, t_object *skip_object)
{
	int		j;
	double	t;
	t_hit	hit;

	j = 0;
	while (j < mini->scene.object_count)
	{
		if (!skip_object || &mini->scene.objects[j] != skip_object)
		{
			t = mini->scene.objects[j].hit(&mini->scene.objects[j],
					shadow_ray, &hit);
			if (t > 0 && t < dist)
				return (true);
		}
		j++;
	}
	return (false);
}

static bool	is_point_visible(t_miniRT *mini, t_vec3 point,
	t_light light, t_object *skip_object)
{
	t_vec3	samples_pos;
	t_vec3	dir;
	double	dist;
	t_ray	shadow_ray;

	samples_pos = random_points(light.position, 0.2);
	dir = vec_sub(samples_pos, point);
	dist = vec_length(dir);
	dir = vec_normalize(dir);
	shadow_ray.origin = vec_add(point, vec_mul(dir, 1e-4));
	shadow_ray.direction = dir;
	return (!is_shadow_blocked(mini, shadow_ray, dist, skip_object));
}

// Modified to accept a skip_object parameter
static double	compute_shadow_factor(t_miniRT *mini, t_vec3 point,
	t_light light, t_object *skip_object)
{
	int			unblocked;
	int			i;
	double		shadow_ratio;

	unblocked = 0;
	i = 0;
	while (i < mini->samples)
	{
		if (is_point_visible(mini, point, light, skip_object))
			unblocked++;
		i++;
	}
	shadow_ratio = (double)unblocked / (double)mini->samples;
	return (shadow_ratio);
}

static t_color	add_light_contribution(t_miniRT *mini, t_hit hit,
	t_light light, t_object *skip_object,
	t_color current_color)
{
	t_vec3	light_dir;
	double	shadow;
	double	diffuse;
	t_color	light_contrib;

	light_dir = vec_normalize(vec_sub(light.position, hit.point));
	shadow = compute_shadow_factor(mini, hit.point, light, skip_object);
	shadow = pow(shadow, 0.7);
	if (shadow <= 0.0)
		return (current_color);
	diffuse = fmax(0.0, vec_skal(hit.normal, light_dir));
	light_contrib = color_scale(hit.color, diffuse * light.brightness * shadow);
	light_contrib = color_mix(light_contrib, light.color, 0.2);
	return (color_add(current_color, light_contrib));
}

static t_color	compute_all_lights(t_miniRT *mini, t_hit hit,
	t_object *skip_object, t_color ambient_color)
{
	int		i;
	t_color	final_color;

	final_color = ambient_color;
	i = 0;
	while (i < mini->scene.light_count)
	{
		final_color = add_light_contribution(mini, hit,
				mini->scene.lights[i], skip_object, final_color);
		i++;
	}
	return (color_clamp(final_color));
}

static t_color	compute_default_lighting(t_miniRT *mini, t_hit hit)
{
	t_vec3	default_light_dir;
	double	diffuse;
	t_color	light_contrib;
	t_color	final_color;

	default_light_dir = (t_vec3){0, -1, 0};
	diffuse = fmax(0.0, vec_skal(hit.normal, vec_neg(default_light_dir)));
	light_contrib = color_scale(hit.color, diffuse * 0.7);
	final_color = color_add(color_scale(mini->scene.ambient.color,
				mini->scene.ambient.ratio), light_contrib);
	return (color_clamp(final_color));
}

// New function that can skip an object during shadow calculations
t_color	compute_lighting_skip_object(t_miniRT *mini, t_hit hit,
	t_object *skip_object)
{
	t_color	final_color;

	final_color = color_scale(mini->scene.ambient.color,
			mini->scene.ambient.ratio);
	if (mini->scene.light_count == 0)
		return (compute_default_lighting(mini, hit));	
	return (compute_all_lights(mini, hit, skip_object, final_color));
	}

t_color	compute_lighting(t_miniRT *mini, t_hit hit)
{
	return (compute_lighting_skip_object(mini, hit, NULL));
}
