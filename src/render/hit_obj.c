/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_obj.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 16:28:26 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/12 11:54:42 by nlewicki         ###   ########.fr       */
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

t_color checkerboard_plane(t_plane *plane, t_vec3 point)
{
	// Set default checkerboard colors
	plane->checker_white = (t_color){255, 255, 255, 255};  // White
	plane->checker_black = (t_color){0, 0, 0, 255};        // Black

	// Create a coordinate system on the plane
	t_vec3 normal = plane->orientation;
	t_vec3 u_axis, v_axis;

	// Find perpendicular vectors to the normal
	if (fabs(normal.x) < fabs(normal.y) && fabs(normal.x) < fabs(normal.z))
		u_axis = (t_vec3){0, -normal.z, normal.y};
	else if (fabs(normal.y) < fabs(normal.z))
		u_axis = (t_vec3){-normal.z, 0, normal.x};
	else
		u_axis = (t_vec3){-normal.y, normal.x, 0};

	u_axis = vec_normalize(u_axis);
	v_axis = vec_cross(normal, u_axis);

	// Project the hit point onto the plane's coordinate system
	t_vec3 relative_pos = vec_sub(point, plane->position);
	double u = vec_dot(relative_pos, u_axis);
	double v = vec_dot(relative_pos, v_axis);

	// Create checkerboard pattern (adjust scale as needed)
	double scale = 1.0; // Adjust this value to change the size of the squares
	int u_int = (int)(u / scale);
	int v_int = (int)(v / scale);

	if ((u_int + v_int) % 2 == 0)
		return plane->checker_black;
	else
		return plane->checker_white;
}

t_color checkerboard_cylinder(t_cylinder *cyl, t_vec3 point)
{
	// Default checkerboard colors
	cyl->checker_white = (t_color){255, 255, 255, 255};
	cyl->checker_black = (t_color){0, 0, 0, 255};

	// Build local cylinder space
	t_vec3 axis = vec_normalize(cyl->orientation);
	t_vec3 cp = vec_sub(point, cyl->position);

	// Project point onto cylinder axis to get height (v)
	double height = vec_dot(cp, axis);

	// Reject the axis component to get radial direction
	t_vec3 radial = vec_sub(cp, vec_mul(axis, height));
	radial = vec_normalize(radial);

	// Get angle around the axis (u)
	double theta = atan2(radial.z, radial.x); // adjust axes as needed
	if (theta < 0)
		theta += 2 * M_PI;
	double u = theta / (2 * M_PI); // 0 to 1
	double v = height / cyl->height; // 0 to 1

	// Scale u and v to create checker pattern
	int u_int = (int)(u * 12); // adjust scale
	int v_int = (int)(v * 6);

	if ((u_int + v_int) % 2 == 0)
		return cyl->checker_black;
	else
		return cyl->checker_white;
}

t_color checkerboard_cone(t_cone *cone, t_vec3 point)
{
	// Default checkerboard colors
	cone->checker_white = (t_color){255, 255, 255, 255};
	cone->checker_black = (t_color){0, 0, 0, 255};

	// Get vector from apex to point
	t_vec3 ap = vec_sub(point, cone->apex);

	// Get height along cone axis
	double height = vec_dot(ap, cone->direction);
	double v = height / cone->height; // 0 to 1

	// Project point onto plane perpendicular to cone axis
	t_vec3 radial = vec_sub(ap, vec_mul(cone->direction, height));
	radial = vec_normalize(radial);

	// Get angle around the axis
	double theta = atan2(radial.z, radial.x);
	if (theta < 0)
		theta += 2 * M_PI;
	double u = theta / (2 * M_PI); // 0 to 1

	// Scale u and v to create checker pattern
	// Use more squares around circumference due to cone expanding
	int u_int = (int)(u * 16);
	int v_int = (int)(v * 8);

	if ((u_int + v_int) % 2 == 0)
		return cone->checker_black;
	else
		return cone->checker_white;
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
	if (sphere->texture.enabled)
		hit->color = get_texture_color(&sphere->texture, hit->point, obj);
	else if (sphere->checker)
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
	if (plane->texture.enabled)
		hit->color = get_texture_color(&plane->texture, hit->point, obj);
	else if (plane->checker)
		hit->color = checkerboard_plane(plane, hit->point);
	else
		hit->color = obj->color;
	hit->object = obj;
	return (t);
}
