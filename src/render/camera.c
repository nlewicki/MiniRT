/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:20:32 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/22 11:22:35 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Calculate viewport dimensions and pixel coordinates
void	calculate_viewport(t_camera cam, int x, int y, double *coords)
{
	double	aspect_ratio;
	double	fov_rad;
	double	viewport_height;
	double	viewport_width;

	aspect_ratio = (double)WIDTH / (double)HEIGHT;
	fov_rad = (cam.fov * M_PI) / 180.0;
	viewport_height = 2.0 * tan(fov_rad / 2.0);
	viewport_width = viewport_height * aspect_ratio;
	coords[0] = ((double)x + 0.5) / WIDTH * 2.0 - 1.0;
	coords[1] = (1.0 - ((double)y + 0.5) / HEIGHT) * 2.0 - 1.0;
	coords[2] = coords[0] * (viewport_width / 2.0);
	coords[3] = coords[1] * (viewport_height / 2.0);
}

// Calculate camera basis vectors
void	calculate_camera_basis(t_camera cam, t_vec3 *basis)
{
	t_vec3	up;

	basis[0] = vec_normalize(cam.orientation);
	up = (t_vec3){0, 1, 0};
	basis[1] = vec_normalize(vec_cross(basis[0], up));
	basis[2] = vec_cross(basis[1], basis[0]);
}

// Generate a ray from the camera through a pixel
t_ray	generate_camera_ray(t_camera cam, int x, int y)
{
	double	coords[4];
	t_vec3	basis[3];
	t_vec3	ray_dir;
	t_ray	ray;

	calculate_viewport(cam, x, y, coords);
	calculate_camera_basis(cam, basis);
	ray_dir = vec_add(basis[0], vec_add(
		vec_mul(basis[1], coords[2]),
		vec_mul(basis[2], coords[3])));
	ray_dir = vec_normalize(ray_dir);
	ray.origin = cam.position;
	ray.direction = ray_dir;
	return (ray);
}
