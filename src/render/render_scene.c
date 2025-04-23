/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:59:43 by lkubler           #+#    #+#             */
/*   Updated: 2025/04/23 11:08:59 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static t_ray generate_camera_ray(t_camera cam, int x, int y)
{
	double aspect_ratio = (double)WIDTH / (double)HEIGHT;
	double fov_rad = (cam.fov * M_PI) / 180.0;
	double viewport_height = 2.0 * tan(fov_rad / 2.0);
	double viewport_width = viewport_height * aspect_ratio;

	// Normalize pixel coordinates to [-1, 1]
	double u = ((double)x + 0.5) / WIDTH * 2.0 - 1.0;
	double v = (1.0 - ((double)y + 0.5) / HEIGHT) * 2.0 - 1.0;

	// Viewport coordinates
	double px = u * (viewport_width / 2.0);
	double py = v * (viewport_height / 2.0);

	// Create camera space basis vectors
	t_vec3 forward = vec_normalize(cam.orientation);
	t_vec3 up = (t_vec3){0, 1, 0}; // World up vector
	t_vec3 right = vec_normalize(vec_cross(forward, up));
	up = vec_cross(right, forward); // Recalculate up to ensure orthogonality

	// Combine the basis vectors to get the final ray direction
	t_vec3 ray_dir = vec_add(
		vec_add(
			vec_mul(right, px),
			vec_mul(up, py)
		),
		forward
	);
	ray_dir = vec_normalize(ray_dir);

	t_ray ray;
	ray.origin = cam.position;
	ray.direction = ray_dir;

	return (ray);
}

uint32_t color_to_uint32(t_color color)
{
	return ((color.r << 24) | (color.g << 16) | (color.b << 8) | color.a);
}


void render_scene(mlx_image_t *img, t_miniRT *mini)
{
	double	closest;
	t_hit	closest_hit;
	bool	hit_any;
	t_scene *scene = &mini->scene;
	int scale = mini->low_res_mode ? mini->res_scale : 1;

	// Loop through pixels with current resolution scale
	for (int y = 0; y < HEIGHT; y += scale)
	{
		for (int x = 0; x < WIDTH; x += scale)
		{
			closest = 1e30;
			hit_any = false;
			t_ray ray = generate_camera_ray(scene->camera, x, y);

			u_int32_t pixel_color = color_to_uint32(scene->ambient.color);

			for (int i = 0; i < scene->object_count; i++)
			{
				t_hit temp_hit;
				double t = scene->objects[i].hit(&scene->objects[i], ray, &temp_hit);
				if (t > 0 && t < closest)
				{
					closest = t;
					closest_hit = temp_hit;
					hit_any = true;
				}
			}

			if (hit_any)
				pixel_color = color_to_uint32(compute_lighting(scene, closest_hit));

			// Fill block of pixels with the same color
			for (int dy = 0; dy < scale && y + dy < HEIGHT; dy++)
			{
				for (int dx = 0; dx < scale && x + dx < WIDTH; dx++)
				{
					mlx_put_pixel(img, x + dx, y + dy, pixel_color);
				}
			}
		}
	}
}
