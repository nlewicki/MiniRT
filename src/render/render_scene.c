/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:59:43 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/23 11:03:26 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

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

//static t_color apply_gamma(t_color c, double gamma)
//{
//	t_color result;

//	result.r = pow(fmin(1.0, c.r / 255.0), 1.0 / gamma) * 255.0;
//	result.g = pow(fmin(1.0, c.g / 255.0), 1.0 / gamma) * 255.0;
//	result.b = pow(fmin(1.0, c.b / 255.0), 1.0 / gamma) * 255.0;
//	result.a = c.a;
//	return result;
//}
