/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:59:43 by lkubler           #+#    #+#             */
/*   Updated: 2025/04/25 13:31:07 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color color_clamp(t_color c)
{
	c.r = fmin(255, fmax(0, c.r));
	c.g = fmin(255, fmax(0, c.g));
	c.b = fmin(255, fmax(0, c.b));
	return c;
}

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

t_color trace_ray(t_miniRT *mini, t_ray ray, int depth)
{
	if (depth <= 0)
		return color_scale(mini->scene.ambient.color, mini->scene.ambient.ratio);

	double closest = 1e30;
	t_hit closest_hit;
	bool hit_any = false;
	t_object *hit_object = NULL;

	// Durch alle Objekte iterieren
	for (int i = 0; i < mini->scene.object_count; i++)
	{
		t_hit temp_hit;
		double t = mini->scene.objects[i].hit(&mini->scene.objects[i], ray, &temp_hit);
		if (t > 0 && t < closest)
		{
			closest = t;
			closest_hit = temp_hit;
			hit_object = &mini->scene.objects[i];
			hit_any = true;
		}
	}

	if (hit_any)
	{
		t_color local_color = compute_lighting(mini, closest_hit); // Beleuchtung am Punkt berechnen

		// Reflexionseigenschaft des getroffenen Objekts holen
		double reflection = 0;
		if (hit_object->type == SPHERE)
			reflection = ((t_sphere *)hit_object->data)->reflection;
		else if (hit_object->type == PLANE)
			reflection = ((t_plane *)hit_object->data)->reflection;
		else if (hit_object->type == CYLINDER)
			reflection = ((t_cylinder *)hit_object->data)->reflection;

		// Wenn Reflexion gewünscht ist (reflection > 0)
		if (reflection > 0)
		{
			// Spiegelstrahl berechnen
			t_vec3 reflect_dir = vec_reflect(ray.direction, closest_hit.normal);
			t_ray reflect_ray = {
				.origin = vec_add(closest_hit.point, vec_mul(reflect_dir, 1e-4)),
				.direction = reflect_dir
			};

			// Rekursiv den reflektierten Strahl verfolgen
			t_color reflected_color = trace_ray(mini, reflect_ray, depth - 1);

			// Mischung der lokalen Farbe mit der reflektierten Farbe
			t_color final_color = color_mix(local_color, reflected_color, reflection);
			return color_clamp(final_color);
		}
		else
		{
			// Wenn keine Reflexion, nur matte Farbe
			return local_color;
		}
	}
	else
	{
		// Kein Objekt getroffen → Hintergrundfarbe
		return color_scale(mini->scene.ambient.color, mini->scene.ambient.ratio);
	}
}


uint32_t color_to_uint32(t_color color)
{
	return ((color.r << 24) | (color.g << 16) | (color.b << 8) | color.a);
}

//static t_color apply_gamma(t_color c, double gamma)
//{
//	t_color result;

//	result.r = pow(fmin(1.0, c.r / 255.0), 1.0 / gamma) * 255.0;
//	result.g = pow(fmin(1.0, c.g / 255.0), 1.0 / gamma) * 255.0;
//	result.b = pow(fmin(1.0, c.b / 255.0), 1.0 / gamma) * 255.0;
//	result.a = c.a;
//	return result;
//}


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
			{
				t_color lit = trace_ray(mini, ray, 3);  // Max. Rekursionstiefe = 3
				lit = color_clamp(lit);
				pixel_color = color_to_uint32(lit);
			}



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
