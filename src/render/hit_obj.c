/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_obj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:28:26 by lkubler           #+#    #+#             */
/*   Updated: 2025/04/28 12:31:25 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

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
	hit->color = obj->color;
	hit->ks = obj->scene->ks;
	hit->shine = obj->scene->shine;
	hit->reflection = obj->scene->reflection;
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
	hit->ks = obj->scene->ks;             // Use current material properties
	hit->shine = obj->scene->shine;
	hit->reflection = obj->scene->reflection;
	return (t);
}
