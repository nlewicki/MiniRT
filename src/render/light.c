/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:40:34 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/27 16:53:20 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Get ambient contribution for mixed lighting
static t_color	get_ambient_contribution(t_miniRT *mini, t_hit hit)
{
	t_color	ambient;

	if (!mini->scene.ambient.is_set || mini->scene.ambient.ratio <= 0.0)
		return ((t_color){0, 0, 0, 255});
	ambient.r = (hit.color.r * mini->scene.ambient.color.r / 255.0)
		* mini->scene.ambient.ratio;
	ambient.g = (hit.color.g * mini->scene.ambient.color.g / 255.0)
		* mini->scene.ambient.ratio;
	ambient.b = (hit.color.b * mini->scene.ambient.color.b / 255.0)
		* mini->scene.ambient.ratio;
	ambient.a = hit.color.a;
	return (ambient);
}

// Calculate light factors with distance attenuation
double	calculate_light_factors(t_light_context ctx, t_light light)
{
	t_vec3	light_vec;
	t_vec3	light_dir;
	double	distance;
	double	shadow;
	double	diff_att[2];

	light_vec = vec_sub(light.position, ctx.hit.point);
	distance = vec_length(light_vec);
	light_dir = vec_normalize(light_vec);
	diff_att[1] = 1.0 / (1.0 + 0.05 * distance + 0.005 * distance * distance);
	shadow = compute_shadow_factor(ctx.mini, ctx.hit.point, light,
			ctx.skip_object);
	shadow = pow(shadow, 0.7);
	if (shadow <= 0.0)
		return (-1.0);
	diff_att[0] = fmax(0.0, vec_dot(ctx.hit.normal, light_dir));
	return (diff_att[0] * light.brightness * shadow * diff_att[1]);
}

// Add light contribution with enhanced colored lighting
static t_color	add_light_contribution(t_light_context ctx, t_light light)
{
	double	light_factor;
	t_color	light_contrib;

	light_factor = calculate_light_factors(ctx, light);
	if (light_factor < 0.0)
		return (ctx.current_color);
	light_contrib = calculate_colored_light(ctx.hit, light, light_factor);
	return (color_add(ctx.current_color, light_contrib));
}

// Compute lighting from all lights
static t_color	compute_all_lights(t_miniRT *mini, t_hit hit,
	t_object *skip_object, t_color ambient_color)
{
	int				i;
	t_light_context	ctx;

	ctx.mini = mini;
	ctx.hit = hit;
	ctx.skip_object = skip_object;
	ctx.current_color = ambient_color;
	i = 0;
	while (i < mini->scene.light_count)
	{
		ctx.current_color = add_light_contribution(ctx, mini->scene.lights[i]);
		i++;
	}
	return (color_clamp(ctx.current_color));
}

// Main lighting function with improved logic
t_color	compute_lighting_skip_object(t_miniRT *mini, t_hit hit,
	t_object *skip_object)
{
	t_color	ambient_color;

	if (mini->scene.light_count == 0)
		return (compute_ambient_only(mini, hit));
	ambient_color = get_ambient_contribution(mini, hit);
	return (compute_all_lights(mini, hit, skip_object, ambient_color));
}
