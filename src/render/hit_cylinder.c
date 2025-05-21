/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:50:00 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/13 11:28:44 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	solve_quadratic(double a, double b, double c)
{
	double	discriminant;
	double	q;
	double	t1;
	double	t2;
	double	tmp;

	if (fabs(a) < 1e-6)
		return (-1.0);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	if (b > 0)
		q = -0.5 * (b + sqrt(discriminant));
	else
		q = -0.5 * (b - sqrt(discriminant));
	t1 = q / a;
	t2 = c / q;
	if (t1 > t2)
	{
		tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	if (t1 > 0)
		return (t1);
	if (t2 > 0)
		return (t2);
	return (-1.0);
}

static double cap_denom(const t_ray ray, t_vec3 normal) {
	return vec_dot(ray.direction, normal);
}

static double cap_t(const t_ray ray, t_vec3 cap_center, t_vec3 normal, double denom) {
	return vec_dot(vec_sub(cap_center, ray.origin), normal) / denom;
}

static int cap_within_radius(t_vec3 p, t_vec3 cap_center, double diameter) {
	return (vec_length(vec_sub(p, cap_center)) <= (diameter / 2) + 1e-6);
}

static void fill_cap_hitinfo(t_hit *hit_info, t_object *obj, t_vec3 p, double t, t_vec3 normal, double denom) {
	hit_info->t = t;
	hit_info->point = p;
	hit_info->normal = (denom > 0) ? normal : vec_mul(normal, -1);
	hit_info->object = obj;
}

static double hit_cylinder_caps(t_object *obj, t_cylinder *cyl, const t_ray ray, t_hit *hit_info, t_vec3 cap_center) {
	double vals[2]; // [0]=denom, [1]=t
	t_vec3 normal = cyl->orientation;
	vals[0] = cap_denom(ray, normal);
	if (fabs(vals[0]) < 1e-6)
		return (-1.0);
	vals[1] = cap_t(ray, cap_center, normal, vals[0]);
	if (vals[1] < 0)
		return (-1.0);
	t_vec3 p = vec_add(ray.origin, vec_mul(ray.direction, vals[1]));
	if (!cap_within_radius(p, cap_center, cyl->diameter))
		return (-1.0);
	if (hit_info)
		fill_cap_hitinfo(hit_info, obj, p, vals[1], normal, vals[0]);
	return (vals[1]);
}

static void compute_caps(t_object *obj, t_cylinder *cyl, const t_ray ray, t_hit *hit_info, double *t, t_vec3 *bottom_cap, t_vec3 *top_cap) {
	double t_cap;
	t_cap = hit_cylinder_caps(obj, cyl, ray, NULL, *bottom_cap);
	if (t_cap > 0 && t_cap < *t) {
		*t = t_cap;
		if (hit_info)
			hit_cylinder_caps(obj, cyl, ray, hit_info, *bottom_cap);
	}
	t_cap = hit_cylinder_caps(obj, cyl, ray, NULL, *top_cap);
	if (t_cap > 0 && t_cap < *t) {
		*t = t_cap;
		if (hit_info)
			hit_cylinder_caps(obj, cyl, ray, hit_info, *top_cap);
	}
}

static void fill_body_hitinfo(t_cylinder *cyl, t_object *obj, t_hit *hit_info, double t, t_vec3 p, double height) {
	hit_info->t = t;
	hit_info->point = p;
	hit_info->normal = vec_normalize(vec_sub(p, vec_add(cyl->position, vec_mul(cyl->orientation, height))));
	if (cyl->checker)
		hit_info->color = checkerboard_cylinder(cyl, hit_info->point);
	else
		hit_info->color = obj->color;
	hit_info->object = obj;
}

double hit_cylinder(t_object *obj, const t_ray ray, t_hit *hit_info) {
	double t = INFINITY, vals[3]; // [0]=a, [1]=b, [2]=c
	t_cylinder *cyl = (t_cylinder *)obj->data;
	if (!cyl) return (-1.0);
	cyl->orientation = vec_normalize(cyl->orientation);
	t_vec3 caps[2] = {cyl->position, vec_add(cyl->position, vec_mul(cyl->orientation, cyl->height))};
	compute_caps(obj, cyl, ray, hit_info, &t, &caps[0], &caps[1]);
	t_vec3 oc = vec_sub(ray.origin, cyl->position);
	double dot_d_axis = vec_dot(ray.direction, cyl->orientation);
	double dot_oc_axis = vec_dot(oc, cyl->orientation);
	vals[0] = vec_dot(ray.direction, ray.direction) - dot_d_axis * dot_d_axis;
	vals[1] = 2 * (vec_dot(ray.direction, oc) - dot_d_axis * dot_oc_axis);
	vals[2] = vec_dot(oc, oc) - dot_oc_axis * dot_oc_axis - (cyl->diameter * cyl->diameter) / 4;
	double t_body = solve_quadratic(vals[0], vals[1], vals[2]);
	if (t_body > 0 && t_body < t) {
		t_vec3 p = vec_add(ray.origin, vec_mul(ray.direction, t_body));
		double height = vec_dot(vec_sub(p, cyl->position), cyl->orientation);
		if (height >= -1e-6 && height <= cyl->height + 1e-6) {
			t = t_body;
			if (hit_info)
				fill_body_hitinfo(cyl, obj, hit_info, t, p, height);
		}
	}
	if (t == INFINITY)
		return (-1.0);
	return (t);

