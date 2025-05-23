/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:03:49 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/23 12:24:05 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	checkerboard_sphere(t_sphere *sph, t_vec3 point)
{
	t_vec3	local;
	double	uv[2];
	int		uv_int[2];

	local = get_local_coords(point, sph->center);
	sph->checker_white = (t_color){255, 255, 255, 255};
	sph->checker_black = (t_color){0, 0, 0, 255};
	uv[0] = 0.5 + atan2(local.z, local.x) / (2 * M_PI);
	uv[1] = 0.5 - asin(local.y) / M_PI;
	uv_int[0] = (int)(uv[0] * 8);
	uv_int[1] = (int)(uv[1] * 8);
	if ((uv_int[0] + uv_int[1]) % 2 == 0)
		return (sph->checker_black);
	return (sph->checker_white);
}

t_vec3	get_u_axis(t_vec3 normal)
{
	t_vec3	u_axis;

	if (fabs(normal.x) < fabs(normal.y) && fabs(normal.x) < fabs(normal.z))
		u_axis = (t_vec3){0, -normal.z, normal.y};
	else if (fabs(normal.y) < fabs(normal.z))
		u_axis = (t_vec3){-normal.z, 0, normal.x};
	else
		u_axis = (t_vec3){-normal.y, normal.x, 0};
	return (vec_normalize(u_axis));
}

t_color	checkerboard_plane(t_plane *pl, t_vec3 point)
{
	double	checker_size;
	t_vec3	u;
	t_vec3	v;
	t_vec3	rel;
	int		check;

	pl->checker_white = (t_color){255, 255, 255, 255};
	pl->checker_black = (t_color){0, 0, 0, 255};
	checker_size = 0.5;
	u = get_u_axis(pl->orientation);
	v = vec_cross(pl->orientation, u);
	rel = vec_sub(point, pl->position);
	check = ((int)floor(vec_dot(rel, u) / checker_size)
			+ (int)floor(vec_dot(rel, v) / checker_size)) % 2;
	if (check == 0)
		return (pl->checker_white);
	return (pl->checker_black);
}

t_color	checkerboard_cylinder(t_cylinder *cyl, t_vec3 point)
{
	t_vec3	cp;
	double	height;
	double	theta;
	int		uv_int[2];

	cyl->checker_white = (t_color){255, 255, 255, 255};
	cyl->checker_black = (t_color){0, 0, 0, 255};
	cp = vec_sub(point, cyl->position);
	height = vec_dot(cp, vec_normalize(cyl->orientation));
	cp = vec_normalize(vec_sub(cp,
				vec_mul(vec_normalize(cyl->orientation), height)));
	theta = atan2(cp.z, cp.x);
	if (theta < 0)
		theta += 2 * M_PI;
	uv_int[0] = (int)((theta / (2 * M_PI)) * 12);
	uv_int[1] = (int)((height / cyl->height) * 6);
	if ((uv_int[0] + uv_int[1]) % 2 == 0)
		return (cyl->checker_black);
	return (cyl->checker_white);
}

t_color	checkerboard_cone(t_cone *cone, t_vec3 point)
{
	t_vec3	ap;
	double	height;
	double	theta;
	int		uv_int[2];

	cone->checker_white = (t_color){255, 255, 255, 255};
	cone->checker_black = (t_color){0, 0, 0, 255};
	ap = vec_sub(point, cone->apex);
	height = vec_dot(ap, cone->direction);
	ap = vec_normalize(vec_sub(ap, vec_mul(cone->direction, height)));
	theta = atan2(ap.z, ap.x);
	if (theta < 0)
		theta += 2 * M_PI;
	uv_int[0] = (int)((theta / (2 * M_PI)) * 16);
	uv_int[1] = (int)((height / cone->height) * 8);
	if ((uv_int[0] + uv_int[1]) % 2 == 0)
		return (cone->checker_black);
	return (cone->checker_white);
}
