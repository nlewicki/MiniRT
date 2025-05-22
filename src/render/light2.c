/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:55:23 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/22 14:03:33 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_vec3	random_points(t_vec3 center, double radius)
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

bool	is_shadow_blocked(t_miniRT *mini, t_ray shadow_ray,
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

bool	is_point_visible(t_miniRT *mini, t_vec3 point,
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
