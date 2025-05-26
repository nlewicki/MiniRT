/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_obj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolewicki <nicolewicki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:28:26 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/26 15:06:18 by nicolewicki      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	set_sphere_info(t_hit_data *data, t_sphere *sphere, double t)
{
	data->hit_info->t = t;
	data->hit_info->point = vec_add(data->ray.origin,
		vec_mul(data->ray.direction, t));
	data->hit_info->normal = vec_normalize(vec_sub(data->hit_info->point,
		sphere->center));
	if (sphere->checker)
		data->hit_info->color = checkerboard_sphere(sphere,
			data->hit_info->point);
	else
		data->hit_info->color = data->obj->color;
	data->hit_info->object = data->obj;
}

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
	set_sphere_info(&(t_hit_data){hit, obj, NULL, ray}, sphere, t);
	return (t);
}

static void	set_plane_info(t_hit_data *data, t_plane *plane, double t)
{
	data->hit_info->t = t;
	data->hit_info->point = vec_add(data->ray.origin,
		vec_mul(data->ray.direction, t));
	if (vec_dot(data->ray.direction, plane->orientation) > 0)
		data->hit_info->normal = vec_mul(plane->orientation, -1);
	else
		data->hit_info->normal = vec_normalize(plane->orientation);
	data->hit_info->color = data->obj->color;
	if (plane->checker)
		data->hit_info->color = checkerboard_plane(plane,
			data->hit_info->point);
	data->hit_info->object = data->obj;
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
	set_plane_info(&(t_hit_data){hit, obj, NULL, ray}, plane, t);
	return (t);
}
