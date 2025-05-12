/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 14:03:49 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/12 14:04:08 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_color	checkerboard_sphere(t_sphere *sph, t_vec3 point)
{
	t_vec3	local;
	double	u;
	double	v;
	int		u_int;
	int		v_int;

	local = vec_sub(point, sph->center);
	local = vec_normalize(local);
	sph->checker_white = (t_color){255, 255, 255, 255};
	sph->checker_black = (t_color){0, 0, 0, 255};
	u = 0.5 + atan2(local.z, local.x) / (2 * M_PI);
	v = 0.5 - asin(local.y) / M_PI;
	u_int = (int)(u * 8);
	v_int = (int)(v * 8);
	if ((u_int + v_int) % 2 == 0)
		return (sph->checker_black);
	return (sph->checker_white);
}

t_color	checkerboard_plane(t_plane *plane, t_vec3 point)
{
	t_vec3	normal;
	t_vec3	u_axis;
	t_vec3	v_axis;
	t_vec3	relative_pos;
	double	u;
	double	v;
	double	scale;
	int		u_int;
	int		v_int;

	plane->checker_white = (t_color){255, 255, 255, 255};
	plane->checker_black = (t_color){0, 0, 0, 255};
	normal = plane->orientation;
	if (fabs(normal.x) < fabs(normal.y) && fabs(normal.x) < fabs(normal.z))
		u_axis = (t_vec3){0, -normal.z, normal.y};
	else if (fabs(normal.y) < fabs(normal.z))
		u_axis = (t_vec3){-normal.z, 0, normal.x};
	else
		u_axis = (t_vec3){-normal.y, normal.x, 0};
	u_axis = vec_normalize(u_axis);
	v_axis = vec_cross(normal, u_axis);
	relative_pos = vec_sub(point, plane->position);
	u = vec_dot(relative_pos, u_axis);
	v = vec_dot(relative_pos, v_axis);
	scale = 1.0;
	u_int = (int)(u / scale);
	v_int = (int)(v / scale);
	if ((u_int + v_int) % 2 == 0)
		return (plane->checker_black);
	return (plane->checker_white);
}

t_color	checkerboard_cylinder(t_cylinder *cyl, t_vec3 point)
{
	t_vec3	axis;
	t_vec3	cp;
	t_vec3	radial;
	double	height;
	double	theta;
	double	u;
	double	v;
	int		u_int;
	int		v_int;

	cyl->checker_white = (t_color){255, 255, 255, 255};
	cyl->checker_black = (t_color){0, 0, 0, 255};
	axis = vec_normalize(cyl->orientation);
	cp = vec_sub(point, cyl->position);
	height = vec_dot(cp, axis);
	radial = vec_sub(cp, vec_mul(axis, height));
	radial = vec_normalize(radial);
	theta = atan2(radial.z, radial.x);
	if (theta < 0)
		theta += 2 * M_PI;
	u = theta / (2 * M_PI);
	v = height / cyl->height;
	u_int = (int)(u * 12);
	v_int = (int)(v * 6);
	if ((u_int + v_int) % 2 == 0)
		return (cyl->checker_black);
	return (cyl->checker_white);
}

t_color	checkerboard_cone(t_cone *cone, t_vec3 point)
{
	t_vec3	ap;
	t_vec3	radial;
	double	height;
	double	v;
	double	theta;
	double	u;
	int		u_int;
	int		v_int;

	cone->checker_white = (t_color){255, 255, 255, 255};
	cone->checker_black = (t_color){0, 0, 0, 255};
	ap = vec_sub(point, cone->apex);
	height = vec_dot(ap, cone->direction);
	v = height / cone->height;
	radial = vec_sub(ap, vec_mul(cone->direction, height));
	radial = vec_normalize(radial);
	theta = atan2(radial.z, radial.x);
	if (theta < 0)
		theta += 2 * M_PI;
	u = theta / (2 * M_PI);
	u_int = (int)(u * 16);
	v_int = (int)(v * 8);
	if ((u_int + v_int) % 2 == 0)
		return (cone->checker_black);
	return (cone->checker_white);
}
