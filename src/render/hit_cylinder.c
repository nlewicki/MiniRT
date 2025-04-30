/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:50:00 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/30 11:39:01 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static double	hit_cylinder_caps(t_object *obj, t_cylinder *cyl, const t_ray ray,
	t_hit *hit_info, t_vec3 cap_center)
{
	t_vec3	normal;
	double	denom;
	double	t;
	t_vec3	p;

	normal = cyl->orientation;
	denom = vec_dot(ray.direction, normal);
	if (fabs(denom) < 1e-6)
		return (-1.0);
	t = vec_dot(vec_sub(cap_center, ray.origin), normal) / denom;
	if (t < 0)
		return (-1.0);
	p = vec_add(ray.origin, vec_mul(ray.direction, t));
	if (vec_length(vec_sub(p, cap_center)) > (cyl->diameter / 2) + 1e-6)
		return (-1.0);
	if (hit_info)
	{
		hit_info->t = t;
		hit_info->point = p;
		hit_info->normal = normal;
		if (denom > 0)
			hit_info->normal = vec_mul(normal, -1);
		hit_info->object = obj;
	}
	return (t);
}

static double	solve_quadratic(double a, double b, double c)
{
	double	discriminant;
	double	q;
	double	t1;
	double	t2;

	if (fabs(a) < 1e-6)
		return (-1.0);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);

	// Use numerically stable quadratic formula
	q = (b > 0) ?
		-0.5 * (b + sqrt(discriminant)) :
		-0.5 * (b - sqrt(discriminant));
	t1 = q / a;
	t2 = c / q;

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
	return (-1.0);
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
	if (!cyl)
		return (-1.0);

	cyl->orientation = vec_normalize(cyl->orientation);
	bottom_cap = cyl->position;
	top_cap = vec_add(cyl->position, vec_mul(cyl->orientation, cyl->height));

	// Check cylinder caps first
	t = INFINITY;
	t_cap = hit_cylinder_caps(obj, cyl, ray, NULL, bottom_cap);
	if (t_cap > 0 && t_cap < t)
	{
		t = t_cap;
		if (hit_info)
			hit_cylinder_caps(obj, cyl, ray, hit_info, bottom_cap);
	}
	t_cap = hit_cylinder_caps(obj, cyl, ray, NULL, top_cap);
	if (t_cap > 0 && t_cap < t)
	{
		t = t_cap;
		if (hit_info)
			hit_cylinder_caps(obj, cyl, ray, hit_info, top_cap);
	}

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
	if (t_body > 0 && t_body < t)
	{
		p = vec_add(ray.origin, vec_mul(ray.direction, t_body));
		height = vec_dot(vec_sub(p, cyl->position), cyl->orientation);
		// Use epsilon to avoid numerical precision issues at boundaries
		if (height >= -1e-6 && height <= cyl->height + 1e-6)
			t = t_body;
	}

	if (t == INFINITY)
		return (-1.0);

	if (hit_info)
	{
		hit_info->t = t;
		hit_info->point = vec_add(ray.origin, vec_mul(ray.direction, t));

		// If hit is body hit (not caps), compute body normal
		if (t == t_body)
		{
			height = vec_dot(vec_sub(hit_info->point, cyl->position),
				cyl->orientation);
			t_vec3 center_at_height = vec_add(cyl->position,
				vec_mul(cyl->orientation, height));
			hit_info->normal = vec_normalize(
				vec_sub(hit_info->point, center_at_height));
		}
		hit_info->color = obj->color;
		hit_info->object = obj;
	}
	return (t);
}
