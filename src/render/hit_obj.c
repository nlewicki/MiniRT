/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_obj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:28:26 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/22 14:17:48 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

double	hit_sphere(t_object *obj, const t_ray ray, t_hit *hit)
{
	t_sphere	*sphere;
	double		radius;
	t_vec3		oc;
	double		a;
	double		b;
	double		c;
	double		discriminant;
	double		t;

	sphere = (t_sphere *)obj->data;
	radius = sphere->diameter / 2.0;
	oc = vec_sub(ray.origin, sphere->center);
	a = vec_skal(ray.direction, ray.direction);
	b = 2.0 * vec_skal(oc, ray.direction);
	c = vec_skal(oc, oc) - radius * radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	t = (-b - sqrt(discriminant)) / (2.0 * a);
	if (t < 0)
		t = (-b + sqrt(discriminant)) / (2.0 * a);
	if (t < 0)
		return (-1);
	hit->t = t;
	hit->point = vec_add(ray.origin, vec_mul(ray.direction, t));
	hit->normal = vec_normalize(vec_sub(hit->point, sphere->center));
	if (sphere->checker)
		hit->color = checkerboard_sphere(sphere, hit->point);
	else
		hit->color = obj->color;
	hit->object = obj;
	return (t);
}

double	hit_plane(t_object *obj, const t_ray ray, t_hit *hit)
{
	t_plane	*plane;
	double	denom;
	double	t;
	t_vec3	hit_point;
	t_vec3	right;
	t_vec3	forward;
	double	x;
	double	z;

	plane = (t_plane *)obj->data;
	denom = vec_skal(plane->orientation, ray.direction);
	if (fabs(denom) < 1e-6)
		return (-1);
	t = vec_skal(vec_sub(plane->position, ray.origin), plane->orientation) / denom;
	if (t < 0)
		return (-1);
	hit_point = vec_add(ray.origin, vec_mul(ray.direction, t));

	if (plane->limit_width > 0 && plane->limit_height > 0)
	{
		forward = vec_normalize(plane->orientation);
		right = vec_normalize(vec_cross((t_vec3){0, 1, 0}, forward));
		if (fabs(vec_dot(right, right)) < 1e-6)
			right = vec_normalize(vec_cross((t_vec3){1, 0, 0}, forward));

		x = vec_dot(vec_sub(hit_point, plane->position), right);
		z = vec_dot(vec_sub(hit_point, plane->position), vec_cross(forward, right));

		if (fabs(x) > plane->limit_width / 2 || fabs(z) > plane->limit_height / 2)
			return (-1);
	}
	hit->t = t;
	hit->point = hit_point;
	hit->normal = (denom > 0) ? vec_mul(plane->orientation, -1)
		: vec_normalize(plane->orientation);
	hit->color = obj->color;
	if (plane->checker)
		hit->color = checkerboard_plane(plane, hit->point);
	hit->object = obj;
	return (t);
}
