/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leokubler <leokubler@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:55:23 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/26 10:17:39 by leokubler        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	calculate_spherical_coords(double *coords)
{
	double	u;
	double	v;

	u = (double)rand() / RAND_MAX;
	v = (double)rand() / RAND_MAX;
	coords[0] = 2.0 * M_PI * u;
	coords[1] = acos(2.0 * v - 1.0);
}

static t_vec3	spherical_to_cartesian(double theta, double phi, double radius)
{
	t_vec3	point;
	double	sin_phi;

	sin_phi = sin(phi);
	point.x = radius * sin_phi * cos(theta);
	point.y = radius * sin_phi * sin(theta);
	point.z = radius * cos(phi);
	return (point);
}

t_vec3	random_points(t_vec3 center, double radius)
{
	double	coords[2];
	t_vec3	offset;
	t_vec3	result;

	calculate_spherical_coords(coords);
	offset = spherical_to_cartesian(coords[0], coords[1], radius);
	result = vec_add(center, offset);
	return (result);
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
//check point visibility
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
