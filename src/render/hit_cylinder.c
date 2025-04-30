/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:50:00 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/30 12:43:33 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static double	hit_cylinder_caps(t_cylinder *cyl, const t_ray ray,
	t_hit *hit_info, t_vec3 cap_center)
{
	t_vec3	normal;
	double	denom;
	double	t;
	t_vec3	p;

	normal = cyl->orientation;
	denom = vec_dot(ray.direction, normal);
	if (fabs(denom) < 1e-6)
		return (INFINITY);
	t = vec_dot(vec_sub(cap_center, ray.origin), normal) / denom;
	if (t < 0)
		return (INFINITY);
	p = vec_add(ray.origin, vec_mul(ray.direction, t));
	if (vec_length(vec_sub(p, cap_center)) > cyl->diameter / 2)
		return (INFINITY);
	if (hit_info)
	{
		hit_info->t = t;
		hit_info->point = p;
		hit_info->normal = normal;
		if (denom > 0)
			hit_info->normal = vec_mul(normal, -1);
	}
	return (t);
}

static double	solve_quadratic(double a, double b, double c)
{
	double	discriminant;
	double	t1;
	double	t2;

	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (INFINITY);
	t1 = (-b - sqrt(discriminant)) / (2 * a);
	t2 = (-b + sqrt(discriminant)) / (2 * a);
	if (t1 > t2)
	{
		double tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	if (t1 > 0)
		return (t1);
	if (t2 > 0)
		return (t2);
	return (INFINITY);
}

double	hit_cylinder(t_object *obj, const t_ray ray, t_hit *hit_info)
{
	t_cylinder	*cyl;
	t_vec3		oc;
	t_vec3		d;
	double		a;
	double		b;
	double		c;
	double		t;
	t_vec3		p;
	double		height;
	t_vec3		bottom_cap;
	t_vec3		top_cap;
	double		t_cap;

	cyl = (t_cylinder *)obj->data;
	bottom_cap = cyl->position;
	top_cap = vec_add(cyl->position, vec_mul(cyl->orientation, cyl->height));

	// Check cylinder caps first
	t = INFINITY;
	t_cap = hit_cylinder_caps(cyl, ray, NULL, bottom_cap);
	if (t_cap < t)
		t = t_cap;
	t_cap = hit_cylinder_caps(cyl, ray, NULL, top_cap);
	if (t_cap < t)
		t = t_cap;

	// Check cylinder body
	oc = vec_sub(ray.origin, cyl->position);
	d = ray.direction;
	t_vec3 axis = cyl->orientation;

	// Calculate quadratic equation coefficients
	double dot_d_axis = vec_dot(d, axis);
	double dot_oc_axis = vec_dot(oc, axis);

	a = vec_dot(d, d) - dot_d_axis * dot_d_axis;
	b = 2 * (vec_dot(d, oc) - dot_d_axis * dot_oc_axis);
	c = vec_dot(oc, oc) - dot_oc_axis * dot_oc_axis -
		(cyl->diameter * cyl->diameter) / 4;

	double t_body = solve_quadratic(a, b, c);
	if (t_body < t)
	{
		p = vec_add(ray.origin, vec_mul(ray.direction, t_body));
		height = vec_dot(vec_sub(p, cyl->position), cyl->orientation);
		if (height >= 0 && height <= cyl->height)
			t = t_body;
	}

	if (t == INFINITY || t < 0)
		return (INFINITY);

	if (hit_info)
	{
		hit_info->t = t;
		hit_info->point = vec_add(ray.origin, vec_mul(ray.direction, t));

		// If hit is on caps, normal is already set by hit_cylinder_caps
		if (t != t_cap)
		{
			height = vec_dot(vec_sub(hit_info->point, cyl->position),
				cyl->orientation);
			t_vec3 center_at_height = vec_add(cyl->position,
				vec_mul(cyl->orientation, height));
			hit_info->normal = vec_normalize(
				vec_sub(hit_info->point, center_at_height));
		}
		hit_info->color = obj->material->color;
	}
	return (t);
}
