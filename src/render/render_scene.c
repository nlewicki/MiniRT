/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:59:43 by lkubler           #+#    #+#             */
/*   Updated: 2025/04/21 14:35:49 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

double	hit_sphere(const t_sphere *sphere, const t_ray ray)
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

void render_scene(mlx_image_t *img, t_scene *scene)
{
	for (int y = 0; y < HEIGHT; y++)
	{
		for (int x = 0; x < WIDTH; x++)
		{
			t_ray ray = generate_camera_ray(scene->camera, x, y); // Noch zu schreiben

			t_color pixel_color = scene->ambient.color; // Standardfarbe (z.B. Hintergrund)

			for (int i = 0; i < scene->sphere_count; i++)
			{
				if (hit_sphere(&ray, &scene->spheres[i]))
				{
					pixel_color = scene->spheres[i].color;
					break; // Nur erste getroffene Sphäre färben
				}
			}

			put_pixel(img, x, y, pixel_color);
		}
	}
}

