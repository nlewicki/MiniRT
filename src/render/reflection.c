/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:23:23 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/23 11:07:51 by lkubler          ###   ########.fr       */
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

// Get reflection coefficient based on object type
double	get_reflection_coef(t_object *hit_object)
{
	if (!hit_object)
		return (0.0);
	if (hit_object->type == SPHERE)
		return (((t_sphere *)hit_object->data)->reflection);
	else if (hit_object->type == PLANE)
		return (((t_plane *)hit_object->data)->reflection);
	else if (hit_object->type == CYLINDER)
		return (((t_cylinder *)hit_object->data)->reflection);
	else if (hit_object->type == CONE)
		return (((t_cone *)hit_object->data)->reflection);
	return (0.0);
}

// Handle reflection for a hit point
t_color	handle_reflection(t_miniRT *mini, t_hit hit, t_ray ray, int depth)
{
	double	reflection;
	t_ray	reflect_ray;
	t_color	reflected_color;
	t_color	local_color;
	t_color	final_color;

	reflection = get_reflection_coef(hit.object);
	if (reflection <= 0.0 || depth <= 0)
		return (compute_lighting_skip_object(mini, hit, NULL));
	local_color = compute_lighting_skip_object(mini, hit, NULL);
	reflect_ray = create_reflection_ray(hit, ray);
	reflected_color = trace_ray_skip_object(mini, reflect_ray, depth - 1,
			hit.object);
	final_color = color_mix(local_color, reflected_color, reflection);
	return (color_clamp(final_color));
}

// Handle reflection for a hit point, skipping a specific object for shadows
t_color	handle_reflection_skip(t_miniRT *mini, t_hit hit,
							t_reflection_context context)
{
	double	reflection;
	t_ray	reflect_ray;
	t_color	reflected_color;
	t_color	local_color;
	t_color	final_color;

	reflection = get_reflection_coef(hit.object);
	if (reflection <= 0.0 || context.depth <= 0)
		return (compute_lighting_skip_object(mini, hit, context.skip_object));
	local_color = compute_lighting_skip_object(mini, hit, context.skip_object);
	reflect_ray = create_reflection_ray(hit, context.ray);
	reflected_color = trace_ray_skip_object(mini, reflect_ray,
			context.depth - 1, hit.object);
	final_color = color_mix(local_color, reflected_color, reflection);
	return (color_clamp(final_color));
}
