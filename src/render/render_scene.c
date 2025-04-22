/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:59:43 by lkubler           #+#    #+#             */
/*   Updated: 2025/04/22 18:52:26 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static t_ray generate_camera_ray(t_camera cam, int x, int y)
{
	double aspect_ratio = (double)WIDTH / (double)HEIGHT;
	double fov_rad = (cam.fov * M_PI) / 180.0;
	double viewport_height = 2.0 * tan(fov_rad / 2.0);
	double viewport_width = viewport_height * aspect_ratio;

	// Pixel-Normalisierung auf [-1, 1] für X und Y
	double u = ((double)x + 0.5) / WIDTH * 2.0 - 1.0;
	double v = (1.0 - ((double)y + 0.5) / HEIGHT) * 2.0 - 1.0;

	// Viewplane-Koordinaten
	double px = u * (viewport_width / 2.0);
	double py = v * (viewport_height / 2.0);

	// Blickrichtung auf -Z (wenn orientation z.B. (0, 0, -1))
	t_vec3 ray_dir = vec_normalize((t_vec3){px, py, 1.0});

	// Optional: in Kamera-Richtung rotieren (wenn orientation ≠ (0, 0, -1)) → später
	// Aktuell ignorieren wir orientation, um's einfacher zu halten

	t_ray ray;
	ray.origin = cam.position;
	ray.direction = ray_dir;

	return (ray);
}

uint32_t color_to_uint32(t_color color)
{
	return ((color.r << 24) | (color.g << 16) | (color.b << 8) | color.a);
}


void render_scene(mlx_image_t *img, t_scene *scene)
{
	double	closest;
	t_hit	closest_hit;
	bool	hit_any;

	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			closest = 1e30;
			hit_any = false;
			t_ray ray = generate_camera_ray(scene->camera, x, y);

			u_int32_t pixel_color = color_to_uint32(scene->ambient.color); // Standardfarbe (z.B. Hintergrund)

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
			mlx_put_pixel(img, x, y, pixel_color);
		}
	}
}
