/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:50:00 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/08 12:04:35 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	solve_quadratic(double a, double b, double c)
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

static double hit_cylinder_caps(t_object *obj, t_cylinder *cyl, const t_ray ray, t_hit *hit_info, t_vec3 cap_center) {
    t_vec3 normal = cyl->orientation;
    double denom = vec_dot(ray.direction, normal);
    if (fabs(denom) < 1e-6) // Ray is parallel to cap
        return -1.0;
    double t = vec_dot(vec_sub(cap_center, ray.origin), normal) / denom;
    if (t < 0) // Intersection is behind the ray origin
        return -1.0;
    t_vec3 p = vec_add(ray.origin, vec_mul(ray.direction, t));
    if (vec_length(vec_sub(p, cap_center)) > (cyl->diameter / 2) + 1e-6) // Point outside the cap's radius
        return -1.0;

    if (hit_info) {
        hit_info->t = t;
        hit_info->point = p;
        hit_info->normal = (denom > 0) ? normal : vec_mul(normal, -1);
        hit_info->object = obj;
    }
    return t;
}

// Simplified hit cylinder function
double hit_cylinder(t_object *obj, const t_ray ray, t_hit *hit_info) {
    t_cylinder *cyl = (t_cylinder *)obj->data;
    if (!cyl)
        return -1.0;

    cyl->orientation = vec_normalize(cyl->orientation);
    t_vec3 bottom_cap = cyl->position;
    t_vec3 top_cap = vec_add(cyl->position, vec_mul(cyl->orientation, cyl->height));

    // Check the bottom and top caps
    double t = INFINITY;
    double t_cap = hit_cylinder_caps(obj, cyl, ray, NULL, bottom_cap);
    if (t_cap > 0 && t_cap < t) {
        t = t_cap;
        if (hit_info)
            hit_cylinder_caps(obj, cyl, ray, hit_info, bottom_cap);
    }

    t_cap = hit_cylinder_caps(obj, cyl, ray, NULL, top_cap);
    if (t_cap > 0 && t_cap < t) {
        t = t_cap;
        if (hit_info)
            hit_cylinder_caps(obj, cyl, ray, hit_info, top_cap);
    }

    // Calculate intersection with cylinder body
    t_vec3 oc = vec_sub(ray.origin, cyl->position);
    t_vec3 d = ray.direction;
    t_vec3 axis = cyl->orientation;

    double dot_d_axis = vec_dot(d, axis);
    double dot_oc_axis = vec_dot(oc, axis);

    double a = vec_dot(d, d) - dot_d_axis * dot_d_axis;
    double b = 2 * (vec_dot(d, oc) - dot_d_axis * dot_oc_axis);
    double c = vec_dot(oc, oc) - dot_oc_axis * dot_oc_axis - (cyl->diameter * cyl->diameter) / 4;

    double t_body = solve_quadratic(a, b, c);
    if (t_body > 0 && t_body < t) {
        t_vec3 p = vec_add(ray.origin, vec_mul(ray.direction, t_body));
        double height = vec_dot(vec_sub(p, cyl->position), cyl->orientation);
        if (height >= -1e-6 && height <= cyl->height + 1e-6) {
            t = t_body;
            if (hit_info) {
                hit_info->t = t;
                hit_info->point = p;
                hit_info->normal = vec_normalize(vec_sub(p, vec_add(cyl->position, vec_mul(cyl->orientation, height))));
                if (cyl->checker)
                    hit_info->color = checkerboard_cylinder(cyl, hit_info->point);
                else
                    hit_info->color = obj->color;
                hit_info->object = obj;
            }
        }
    }

    return (t == INFINITY) ? -1.0 : t;
}
