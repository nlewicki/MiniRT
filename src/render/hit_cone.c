/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolewicki <nicolewicki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:01:08 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/26 12:51:59 by nicolewicki      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static double	cone_body_intersection(t_cone *cone, t_ray ray, t_vec3 oc)
{
	double	cos2;
	double	a;
	double	b;
	double	c;

	cos2 = cos(cone->angle) * cos(cone->angle);
	a = vec_dot(ray.direction, ray.direction) * cos2
		- pow(vec_dot(ray.direction, cone->direction), 2);
	b = 2 * (vec_dot(ray.direction, oc) * cos2 - vec_dot(ray.direction,
				cone->direction) * vec_dot(oc, cone->direction));
	c = vec_dot(oc, oc) * cos2 - pow(vec_dot(oc, cone->direction), 2);
	return (solve_quadratic(a, b, c));
}

static void	set_cone_hit_info(t_hit_data *data, double t)
{
	t_vec3	p;
	t_vec3	cp;
	double	h_ratio;
	t_vec3	proj;

	data->hit_info->t = t;
	p = vec_add(data->ray.origin, vec_mul(data->ray.direction, t));
	data->hit_info->point = p;
	cp = vec_sub(p, data->cone->apex);
	h_ratio = vec_dot(cp, data->cone->direction);
	proj = vec_mul(data->cone->direction, h_ratio);
	data->hit_info->normal = vec_normalize(vec_sub(cp, proj));
	data->hit_info->object = data->obj;
	if (data->cone->checker)
		data->hit_info->color = checkerboard_cone(data->cone, p);
	else
		data->hit_info->color = data->obj->color;
}

static double	hit_cone_base(t_object *obj, t_cone *cone, t_ray ray,
		t_hit *hit_info)
{
	t_vec3	base_center;
	t_vec3	normal;
	double	denom;
	double	t;

	normal = vec_mul(cone->direction, -1);
	base_center = vec_add(cone->apex, vec_mul(cone->direction, cone->height));
	denom = vec_dot(ray.direction, normal);
	if (fabs(denom) < 1e-6)
		return (-1.0);
	t = vec_dot(vec_sub(base_center, ray.origin), normal) / denom;
	if (t < 0)
		return (-1.0);
	if (vec_length(vec_sub(vec_add(ray.origin, vec_mul(ray.direction, t)),
				base_center)) > tan(cone->angle) * cone->height + 1e-6)
		return (-1.0);
	if (hit_info)
	{
		hit_info->t = t;
		hit_info->point = vec_add(ray.origin, vec_mul(ray.direction, t));
		set_base_hit_normal(hit_info, normal, denom);
		hit_info->object = obj;
		check_mode(hit_info, cone, obj);
	}
	return (t);
}

static double	check_cone_body_hit(t_object *obj, t_cone *cone, t_ray ray,
		t_hit *hit_info)
{
	t_vec3		oc;
	double		t;
	double		h;
	t_hit_data	data;

	oc = vec_sub(ray.origin, cone->apex);
	t = cone_body_intersection(cone, ray, oc);
	if (t <= 0)
		return (-1.0);
	h = vec_dot(vec_sub(vec_add(ray.origin, vec_mul(ray.direction, t)),
				cone->apex), cone->direction);
	if (h < 0 || h > cone->height)
		return (-1.0);
	if (hit_info)
	{
		data.hit_info = hit_info;
		data.obj = obj;
		data.cone = cone;
		data.ray = ray;
		set_cone_hit_info(&data, t);
	}
	return (t);
}

double	hit_cone(t_object *obj, const t_ray ray, t_hit *hit_info)
{
	t_cone	*cone;
	double	t[2];

	cone = (t_cone *)obj->data;
	if (!cone)
		return (-1.0);
	cone->direction = vec_normalize(cone->direction);
	t[0] = check_cone_body_hit(obj, cone, ray, NULL);
	if (t[0] > 0)
	{
		if (hit_info)
			check_cone_body_hit(obj, cone, ray, hit_info);
		return (t[0]);
	}
	if (cone->height > 0)
	{
		t[1] = hit_cone_base(obj, cone, ray, NULL);
		if (t[1] > 0)
		{
			if (hit_info)
				hit_cone_base(obj, cone, ray, hit_info);
			return (t[1]);
		}
	}
	return (-1.0);
}
