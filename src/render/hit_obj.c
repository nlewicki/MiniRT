/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_obj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolewicki <nicolewicki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:28:26 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/26 15:17:01 by nicolewicki      ###   ########.fr       */
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
	t_sphere				*sphere;
	t_sphere_intersection	inter;

	sphere = (t_sphere *)obj->data;
	inter.radius = sphere->diameter / 2.0;
	inter.oc = vec_sub(ray.origin, sphere->center);
	inter.a = vec_skal(ray.direction, ray.direction);
	inter.b = 2.0 * vec_skal(inter.oc, ray.direction);
	inter.c = vec_skal(inter.oc, inter.oc) - inter.radius * inter.radius;
	inter.discriminant = inter.b * inter.b - 4 * inter.a * inter.c;
	if (inter.discriminant < 0)
		return (-1.0);
	inter.t = (-inter.b - sqrt(inter.discriminant)) / (2.0 * inter.a);
	if (inter.t < 0)
		inter.t = (-inter.b + sqrt(inter.discriminant)) / (2.0 * inter.a);
	if (inter.t < 0)
		return (-1);
	set_sphere_info(&(t_hit_data){hit, obj, NULL, ray}, sphere, inter.t);
	return (inter.t);
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

static bool	check_plane_limits(t_plane *plane, t_plane_intersection *inter)
{
	if (plane->limit_width > 0 && plane->limit_height > 0)
	{
		inter->forward = vec_normalize(plane->orientation);
		inter->right = vec_normalize(vec_cross((t_vec3){0, 1, 0},
					inter->forward));
		if (fabs(vec_dot(inter->right, inter->right)) < 1e-6)
			inter->right = vec_normalize(vec_cross((t_vec3){1, 0, 0},
						inter->forward));
		inter->x = vec_dot(vec_sub(inter->hit_point, plane->position),
				inter->right);
		inter->z = vec_dot(vec_sub(inter->hit_point, plane->position),
				vec_cross(inter->forward, inter->right));
		if (fabs(inter->x) > plane->limit_width / 2
			|| fabs(inter->z) > plane->limit_height / 2)
			return (false);
	}
	return (true);
}

double	hit_plane(t_object *obj, const t_ray ray, t_hit *hit)
{
	t_plane					*plane;
	t_plane_intersection	inter;
	double					t;

	plane = (t_plane *)obj->data;
	inter.denom = vec_skal(plane->orientation, ray.direction);
	if (fabs(inter.denom) < 1e-6)
		return (-1);
	t = vec_skal(vec_sub(plane->position, ray.origin),
			plane->orientation) / inter.denom;
	if (t < 0)
		return (-1);
	inter.hit_point = vec_add(ray.origin, vec_mul(ray.direction, t));
	if (!check_plane_limits(plane, &inter))
		return (-1);
	set_plane_info(&(t_hit_data){hit, obj, NULL, ray}, plane, t);
	return (t);
}
