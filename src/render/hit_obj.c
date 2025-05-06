/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_obj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:28:26 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/06 11:24:56 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color checkerboard_sphere(t_sphere *sph, t_vec3 point)
{
	t_vec3 local = vec_sub(point, sph->center);
	local = vec_normalize(local);
	
	// Use proper color ranges (0-255 instead of 0.0-1.0)
	sph->checker_white = (t_color){255, 255, 255, 255};  // White
	sph->checker_black = (t_color){0, 0, 0, 255};        // Black

	// Calculate UV coordinates on the sphere
	double u = 0.5 + atan2(local.z, local.x) / (2 * M_PI);
	double v = 0.5 - asin(local.y) / M_PI;

	// Adjust the scale for more visible checkerboard (smaller number = larger squares)
	int u_int = (int)(u * 8);
	int v_int = (int)(v * 8);

	if ((u_int + v_int) % 2 == 0)
		return sph->checker_black;
	else
		return sph->checker_white;
}

double hit_sphere(t_object *obj, const t_ray ray, t_hit *hit)
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

double hit_plane(t_object *obj, const t_ray ray, t_hit *hit)
{
	t_plane	*plane;
	double 	denom;
	double	t;

	plane = (t_plane *)obj->data;
	denom = vec_skal(plane->orientation, ray.direction);
	if (fabs(denom) < 1e-6)
		return (-1);
	t = vec_skal(vec_sub(plane->position, ray.origin), plane->orientation) / denom;
	if (t < 0)
		return (-1);
	hit->t = t;
	hit->point = vec_add(ray.origin, vec_mul(ray.direction, t));
	hit->normal = vec_normalize(plane->orientation);
	hit->color = obj->color;
	hit->object = obj;
	return (t);
}
