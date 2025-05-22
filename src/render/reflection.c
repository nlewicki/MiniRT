/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:23:23 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/22 12:56:32 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Create a reflection ray from a hit point
t_ray	create_reflection_ray(t_hit hit, t_ray incident_ray)
{
	t_ray	reflect_ray;
	t_vec3	reflect_dir;

	reflect_dir = vec_reflect(vec_neg(incident_ray.direction), hit.normal);
	reflect_ray.origin = vec_add(hit.point, vec_mul(reflect_dir, 1e-4));
	reflect_ray.direction = reflect_dir;
	return (reflect_ray);
}

// Find closest hit in scene, skipping a specific object
t_hit	find_closest_hit_skip(t_miniRT *mini, t_ray ray, t_object *skip_object,
							t_object **hit_object)
{
	t_hit	closest_hit;
	t_hit	temp_hit;
	double	closest;
	double	t;
	int		i;

	closest = 1e30;
	i = 0;
	*hit_object = NULL;
	while (i < mini->scene.object_count)
	{
		if (&mini->scene.objects[i] != skip_object)
		{
			t = mini->scene.objects[i].hit(&mini->scene.objects[i],
					ray, &temp_hit);
			if (t > 0 && t < closest)
			{
				closest = t;
				closest_hit = temp_hit;
				*hit_object = &mini->scene.objects[i];
			}
		}
		i++;
	}
	return (closest_hit);
}

// Handle reflection for a hit point
t_color	handle_reflection(t_miniRT *mini, t_hit hit, t_ray ray,
						t_object *hit_object, int depth)
{
	double	reflection;
	t_ray	reflect_ray;
	t_color	reflected_color;
	t_color	local_color;
	t_color	final_color;

	reflection = get_reflection_coef(hit_object);
	if (reflection <= 0.0 || depth <= 0)
		return (compute_lighting_skip_object(mini, hit, NULL));
	local_color = compute_lighting_skip_object(mini, hit, NULL);
	reflect_ray = create_reflection_ray(hit, ray);
	reflected_color = trace_ray_skip_object(mini, reflect_ray, depth - 1,
			hit_object);
	final_color = color_mix(local_color, reflected_color, reflection);
	return (color_clamp(final_color));
}

// Trace a ray while skipping a specific object
t_color	trace_ray_skip_object(t_miniRT *mini, t_ray ray, int depth,
							t_object *skip_object)
{
	t_hit		closest_hit;
	t_object	*hit_object;
	bool		hit_any;

	if (depth <= 0)
		return (color_scale(mini->scene.ambient.color,
				mini->scene.ambient.ratio));
	closest_hit = find_closest_hit_skip(mini, ray, skip_object, &hit_object);
	hit_any = (hit_object != NULL);
	if (hit_any)
	{
		return (handle_reflection_skip(mini, closest_hit, ray, hit_object,
				depth, skip_object));
	}
	if (!mini->scene.ambient.is_set || mini->scene.ambient.ratio <= 0.0)
		return ((t_color){30, 30, 30, 255});
	return (color_scale(mini->scene.ambient.color, mini->scene.ambient.ratio));
}

// Handle reflection for a hit point, skipping a specific object for shadows
t_color	handle_reflection_skip(t_miniRT *mini, t_hit hit, t_ray ray,
							t_object *hit_object, int depth,
								t_object *skip_object)
{
	double	reflection;
	t_ray	reflect_ray;
	t_color	reflected_color;
	t_color	local_color;
	t_color	final_color;

	reflection = get_reflection_coef(hit_object);
	if (reflection <= 0.0 || depth <= 0)
		return (compute_lighting_skip_object(mini, hit, skip_object));
	local_color = compute_lighting_skip_object(mini, hit, skip_object);
	reflect_ray = create_reflection_ray(hit, ray);
	reflected_color = trace_ray_skip_object(mini, reflect_ray, depth - 1,
			hit_object);
	final_color = color_mix(local_color, reflected_color, reflection);
	return (color_clamp(final_color));
}
