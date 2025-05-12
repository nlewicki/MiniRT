/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:01:08 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/12 12:09:28 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static double	hit_cone_base(t_object *obj, t_cone *cone, const t_ray ray,
	t_hit *hit_info)
{
	t_vec3	base_center;
	t_vec3	normal;
	double	denom;
	double	t;
	double	radius;

	normal = vec_mul(cone->direction, -1);
	base_center = vec_add(cone->apex, vec_mul(cone->direction, cone->height));
	denom = vec_dot(ray.direction, normal);
	if (fabs(denom) < 1e-6)
		return (-1.0);
	t = vec_dot(vec_sub(base_center, ray.origin), normal) / denom;
	if (t < 0)
		return (-1.0);
	radius = tan(cone->angle) * cone->height;
	if (vec_length(vec_sub(vec_add(ray.origin, vec_mul(ray.direction, t)),
		base_center)) > radius + 1e-6)
		return (-1.0);
	if (hit_info)
	{
		hit_info->t = t;
		hit_info->point = vec_add(ray.origin, vec_mul(ray.direction, t));
		hit_info->normal = (denom > 0) ? vec_mul(normal, -1) : normal;
		hit_info->object = obj;
		if (cone->texture.enabled)
			hit_info->color = get_texture_color(&cone->texture, hit_info->point, obj);
		else if (cone->checker)
			hit_info->color = checkerboard_cone(cone, hit_info->point);
		else
			hit_info->color = obj->color;
	}
	return (t);
}

double	hit_cone(t_object *obj, const t_ray ray, t_hit *hit_info)
{
	t_cone	*cone;
	t_vec3	oc;
	double	t;
	double	cos2;
	double	a;
	double	b;
	double	c;

	cone = (t_cone *)obj->data;
	if (!cone)
		return (-1.0);
	cone->direction = vec_normalize(cone->direction);
	oc = vec_sub(ray.origin, cone->apex);
	cos2 = cos(cone->angle) * cos(cone->angle);
	a = vec_dot(ray.direction, ray.direction) * cos2
		- pow(vec_dot(ray.direction, cone->direction), 2);
	b = 2 * (vec_dot(ray.direction, oc) * cos2
			- vec_dot(ray.direction, cone->direction)
			* vec_dot(oc, cone->direction));
	c = vec_dot(oc, oc) * cos2 - pow(vec_dot(oc, cone->direction), 2);
	t = solve_quadratic(a, b, c);
	if (t > 0)
	{
		t_vec3	p = vec_add(ray.origin, vec_mul(ray.direction, t));
		double	h = vec_dot(vec_sub(p, cone->apex), cone->direction);
		if (h >= 0 && h <= cone->height)
		{
			if (hit_info)
			{
				hit_info->t = t;
				hit_info->point = p;
				t_vec3	cp = vec_sub(p, cone->apex);
				double	h_ratio = vec_dot(cp, cone->direction);
				t_vec3	proj = vec_mul(cone->direction, h_ratio);
				hit_info->normal = vec_normalize(vec_sub(cp, proj));
				hit_info->object = obj;
				if (cone->texture.enabled)
					hit_info->color = get_texture_color(&cone->texture, hit_info->point, obj);
				else if (cone->checker)
					hit_info->color = checkerboard_cone(cone, hit_info->point);
				else
					hit_info->color = obj->color;
			}
			return (t);
		}
	}

	// Check base cap if cone is truncated
	if (cone->height > 0)
	{
		t = hit_cone_base(obj, cone, ray, NULL);
		if (t > 0)
		{
			if (hit_info)
				hit_cone_base(obj, cone, ray, hit_info);
			return (t);
		}
	}
	return (-1.0);
}
