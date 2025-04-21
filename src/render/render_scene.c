/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:59:43 by lkubler           #+#    #+#             */
/*   Updated: 2025/04/21 15:28:12 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static double	hit_sphere(const t_ray ray, const t_sphere *sphere)
{
	double	radius;
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	discriminant;

	radius = sphere->diameter / 2.0;
	oc = vec_sub(ray.origin, sphere->center);
	a = vec_skal(ray.direction, ray.direction);
	b = 2.0 * vec_skal(oc, ray.direction);
	c = vec_skal(oc, oc) - radius * radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	return ((-b - sqrt(discriminant)) / (2.0 * a));
}


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

	return ray;
}

static uint32_t color_to_uint32(t_color c)
{
	return ((255 << 24) | (c.r << 16) | (c.g << 8) | c.b); // ARGB
}


void render_scene(mlx_image_t *img, t_scene *scene)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			t_ray ray = generate_camera_ray(scene->camera, x, y);

			u_int32_t pixel_color = color_to_uint32(scene->ambient.color); // Standardfarbe (z.B. Hintergrund)

			for (int i = 0; i < scene->sphere_count; i++)
			{
				if (hit_sphere(ray, &scene->spheres[i]) > 0)
				{
					pixel_color = color_to_uint32(scene->spheres[i].color);
					break;
				}
			}
			mlx_put_pixel(img, x, y, pixel_color);
		}
	}
}
