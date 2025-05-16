/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:59:43 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/16 10:46:02 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	color_clamp(t_color c)
{
	c.r = fmin(255, fmax(0, c.r));
	c.g = fmin(255, fmax(0, c.g));
	c.b = fmin(255, fmax(0, c.b));
	return (c);
}

static t_ray	generate_camera_ray(t_camera cam, int x, int y)
{
	double		aspect_ratio;
	double		fov_rad;
	double		viewport_height;
	double		viewport_width;
	double		u;
	double		v;
	double		px;
	double		py;
	t_vec3		forward;
	t_vec3		up;
	t_vec3		right;
	t_vec3		ray_dir;
	t_ray		ray;

	aspect_ratio = (double)WIDTH / (double)HEIGHT;
	fov_rad = (cam.fov * M_PI) / 180.0;
	viewport_height = 2.0 * tan(fov_rad / 2.0);
	viewport_width = viewport_height * aspect_ratio;
	u = ((double)x + 0.5) / WIDTH * 2.0 - 1.0;
	v = (1.0 - ((double)y + 0.5) / HEIGHT) * 2.0 - 1.0;
	px = u * (viewport_width / 2.0);
	py = v * (viewport_height / 2.0);
	forward = vec_normalize(cam.orientation);
	up = (t_vec3){0, 1, 0};
	right = vec_normalize(vec_cross(forward, up));
	up = vec_cross(right, forward);
	ray_dir = vec_add(vec_add(vec_mul(right, px), vec_mul(up, py)), forward);
	ray_dir = vec_normalize(ray_dir);
	ray.origin = cam.position;
	ray.direction = ray_dir;
	return (ray);
}

t_color	trace_ray(t_miniRT *mini, t_ray ray, int depth)
{
	double		closest;
	t_hit		closest_hit;
	bool		hit_any;
	t_object	*hit_object;
	t_hit		temp_hit;
	double		t;
	t_color		local_color;
	double		reflection;
	t_vec3		reflect_dir;
	t_ray		reflect_ray;
	t_color		reflected_color;
	t_color		final_color;
	int			i;

	if (depth <= 0)
		return (color_scale(mini->scene.ambient.color, mini->scene.ambient.ratio));
	closest = 1e30;
	hit_any = false;
	hit_object = NULL;
	i = 0;
	while (i < mini->scene.object_count)
	{
		t = mini->scene.objects[i].hit(&mini->scene.objects[i], ray, &temp_hit);
		if (t > 0 && t < closest)
		{
			closest = t;
			closest_hit = temp_hit;
			hit_object = &mini->scene.objects[i];
			hit_any = true;
		}
		i++;
	}
	if (hit_any)
	{
		local_color = compute_lighting(mini, closest_hit);
		reflection = 0;
		if (hit_object->type == SPHERE)
			reflection = ((t_sphere *)hit_object->data)->reflection;
		else if (hit_object->type == PLANE)
			reflection = ((t_plane *)hit_object->data)->reflection;
		else if (hit_object->type == CYLINDER)
			reflection = ((t_cylinder *)hit_object->data)->reflection;
		else if (hit_object->type == CONE)
			reflection = ((t_cone *)hit_object->data)->reflection;
		if (reflection > 0)
		{
			// FIX: Correct reflection direction calculation
			// The incoming ray direction needs to be negated first
			reflect_dir = vec_reflect(vec_neg(ray.direction), closest_hit.normal);
			reflect_ray.origin = vec_add(closest_hit.point, vec_mul(reflect_dir, 1e-4));
			reflect_ray.direction = reflect_dir;
			
			// Skip the current object when tracing reflection rays to avoid self-intersection
			reflected_color = trace_ray_skip_object(mini, reflect_ray, depth - 1, hit_object);
			
			final_color = color_mix(local_color, reflected_color, reflection);
			return (color_clamp(final_color));
		}
		return (local_color);
	}
	if (!mini->scene.ambient.is_set || mini->scene.ambient.ratio <= 0.0)
		return ((t_color){30, 30, 30, 255});
	return (color_scale(mini->scene.ambient.color, mini->scene.ambient.ratio));
}

// Modified to use compute_lighting_skip_object
t_color trace_ray_skip_object(t_miniRT *mini, t_ray ray, int depth, t_object *skip_object)
{
	double		closest;
	t_hit		closest_hit;
	bool		hit_any;
	t_object	*hit_object;
	t_hit		temp_hit;
	double		t;
	t_color		local_color;
	double		reflection;
	t_vec3		reflect_dir;
	t_ray		reflect_ray;
	t_color		reflected_color;
	t_color		final_color;
	int			i;

	if (depth <= 0)
		return (color_scale(mini->scene.ambient.color, mini->scene.ambient.ratio));
	closest = 1e30;
	hit_any = false;
	hit_object = NULL;
	i = 0;
	while (i < mini->scene.object_count)
	{
		// Skip the object we want to ignore (usually the one we just reflected from)
		if (&mini->scene.objects[i] != skip_object)
		{
			t = mini->scene.objects[i].hit(&mini->scene.objects[i], ray, &temp_hit);
			if (t > 0 && t < closest)
			{
				closest = t;
				closest_hit = temp_hit;
				hit_object = &mini->scene.objects[i];
				hit_any = true;
			}
		}
		i++;
	}
	if (hit_any)
	{
		// Use the new lighting function that also skips the object during shadow calculations
		local_color = compute_lighting_skip_object(mini, closest_hit, skip_object);
		
		reflection = 0;
		if (hit_object->type == SPHERE)
			reflection = ((t_sphere *)hit_object->data)->reflection;
		else if (hit_object->type == PLANE)
			reflection = ((t_plane *)hit_object->data)->reflection;
		else if (hit_object->type == CYLINDER)
			reflection = ((t_cylinder *)hit_object->data)->reflection;
		else if (hit_object->type == CONE)
			reflection = ((t_cone *)hit_object->data)->reflection;
		if (reflection > 0)
		{
			// FIX: Correct reflection direction calculation
			// The incoming ray direction needs to be negated first
			reflect_dir = vec_reflect(vec_neg(ray.direction), closest_hit.normal);
			reflect_ray.origin = vec_add(closest_hit.point, vec_mul(reflect_dir, 1e-4));
			reflect_ray.direction = reflect_dir;
			
			// Continue skipping the original object, but also skip the current object
			reflected_color = trace_ray_skip_object(mini, reflect_ray, depth - 1, hit_object);
			
			final_color = color_mix(local_color, reflected_color, reflection);
			return (color_clamp(final_color));
		}
		return (local_color);
	}
	if (!mini->scene.ambient.is_set || mini->scene.ambient.ratio <= 0.0)
		return ((t_color){30, 30, 30, 255});
	return (color_scale(mini->scene.ambient.color, mini->scene.ambient.ratio));
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

uint32_t	color_to_uint32(t_color color)
{
	return ((color.r << 24) | (color.g << 16) | (color.b << 8) | color.a);
}

void	render_scene(mlx_image_t *img, t_miniRT *mini)
{
	double		closest;
	bool		hit_any;
	t_scene		*scene;
	int			scale;
	t_ray		ray;
	u_int32_t	pixel_color;
	t_hit		temp_hit;
	double		t;
	t_color		lit;
	int			x;
	int			y;
	int			i;
	int			dx;
	int			dy;

	scene = &mini->scene;
	if (mini->low_res_mode)
		scale = mini->res_scale;
	else
		scale = 1;
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			closest = 1e30;
			hit_any = false;
			ray = generate_camera_ray(scene->camera, x, y);
			pixel_color = color_to_uint32(scene->ambient.color);
			i = 0;
			while (i < scene->object_count)
			{
				t = scene->objects[i].hit(&scene->objects[i], ray, &temp_hit);
				if (t > 0 && t < closest)
				{
					closest = t;
					hit_any = true;
				}
				i++;
			}
			if (hit_any)
			{
				lit = trace_ray(mini, ray, 3);
				lit = color_clamp(lit);
				pixel_color = color_to_uint32(lit);
			}
			dy = 0;
			while (dy < scale && y + dy < HEIGHT)
			{
				dx = 0;
				while (dx < scale && x + dx < WIDTH)
				{
					mlx_put_pixel(img, x + dx, y + dy, pixel_color);
					dx++;
				}
				dy++;
			}
			x += scale;
		}
		y += scale;
	}
}
