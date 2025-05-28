/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:59:43 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/28 11:56:09 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Find the closest intersection for a ray
static bool	find_intersection(t_miniRT *mini, t_ray ray, t_hit *hit)
{
	double	closest;
	double	t;
	int		i;
	bool	hit_any;

	closest = 1e30;
	hit_any = false;
	i = 0;
	while (i < mini->scene.object_count)
	{
		t = mini->scene.objects[i].hit(&mini->scene.objects[i], ray, hit);
		if (t > 0 && t < closest)
		{
			closest = t;
			hit_any = true;
		}
		i++;
	}
	return (hit_any);
}

// Calculate the color for a single pixel
static uint32_t	calculate_pixel_color(t_miniRT *mini, int x, int y)
{
	t_ray		ray;
	t_hit		temp_hit;
	bool		hit_any;
	t_color		lit;
	uint32_t	pixel_color;

	ray = generate_camera_ray(mini->scene.camera, x, y);
	pixel_color = color_to_uint32(mini->scene.ambient.color);
	hit_any = find_intersection(mini, ray, &temp_hit);
	if (hit_any)
	{
		lit = trace_ray(mini, ray, 3);
		lit = color_clamp(lit);
		pixel_color = color_to_uint32(lit);
	}
	return (pixel_color);
}

// Fill a block of pixels with the same color (for low-res mode)
static void	fill_pixel_block(t_render_context ctx, uint32_t color)
{
	int	dx;
	int	dy;

	dy = 0;
	while (dy < ctx.scale && ctx.y + dy < HEIGHT)
	{
		dx = 0;
		while (dx < ctx.scale && ctx.x + dx < WIDTH)
		{
			mlx_put_pixel(ctx.img, ctx.x + dx, ctx.y + dy, color);
			dx++;
		}
		dy++;
	}
}

// Process a single row of pixels
static void	process_row(t_render_context ctx)
{
	uint32_t	pixel_color;

	ctx.x = 0;
	while (ctx.x < WIDTH)
	{
		pixel_color = calculate_pixel_color(ctx.mini, ctx.x, ctx.y);
		fill_pixel_block(ctx, pixel_color);
		ctx.x += ctx.scale;
	}
}

// Main render function
void	render_scene(mlx_image_t *img, t_miniRT *mini)
{
	t_render_context	ctx;

	ctx.mini = mini;
	ctx.img = img;
	if (mini->low_res_mode)
		ctx.scale = mini->res_scale;
	else
		ctx.scale = 1;
	ctx.y = 0;
	while (ctx.y < HEIGHT)
	{
		process_row(ctx);
		ctx.y += ctx.scale;
	}
}
