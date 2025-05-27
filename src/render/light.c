/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:40:34 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/27 15:03:34 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Modified to accept a skip_object parameter
double	compute_shadow_factor(t_miniRT *mini, t_vec3 point,
	t_light light, t_object *skip_object)
{
	int			unblocked;
	int			i;
	double		shadow_ratio;

	unblocked = 0;
	i = 0;
	while (i < mini->samples)
	{
		if (is_point_visible(mini, point, light, skip_object))
			unblocked++;
		i++;
	}
	shadow_ratio = (double)unblocked / (double)mini->samples;
	return (shadow_ratio);
}

static t_color	compute_ambient_only(t_miniRT *mini, t_hit hit)
{
	t_color	ambient_contrib;

	if (!mini->scene.ambient.is_set || mini->scene.ambient.ratio <= 0.0)
		return ((t_color){10, 10, 10, 255});
	ambient_contrib.r = (hit.color.r * mini->scene.ambient.color.r / 255.0) 
		* mini->scene.ambient.ratio;
	ambient_contrib.g = (hit.color.g * mini->scene.ambient.color.g / 255.0) 
		* mini->scene.ambient.ratio;
	ambient_contrib.b = (hit.color.b * mini->scene.ambient.color.b / 255.0) 
		* mini->scene.ambient.ratio;
	ambient_contrib.a = hit.color.a;
	return (color_clamp(ambient_contrib));
}

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
	double	attenuation;
	double	shadow;
	double	diffuse;

	light_vec = vec_sub(light.position, ctx.hit.point);
	distance = vec_length(light_vec);
	light_dir = vec_normalize(light_vec);
	attenuation = 1.0 / (1.0 + 0.05 * distance + 0.005 * distance * distance);
	shadow = compute_shadow_factor(ctx.mini, ctx.hit.point, light, 
			ctx.skip_object);
	shadow = pow(shadow, 0.7);
	if (shadow <= 0.0)
		return (-1.0);
	diffuse = fmax(0.0, vec_dot(ctx.hit.normal, light_dir));
	return (diffuse * light.brightness * shadow * attenuation);
}

// Add light contribution with proper color mixing
static t_color	add_light_contribution(t_light_context ctx, t_light light)
{
	double	light_factor;
	t_color	light_contrib;
	t_color	mixed_color;

	light_factor = calculate_light_factors(ctx, light);
	if (light_factor < 0.0)
		return (ctx.current_color);
	light_contrib.r = ctx.hit.color.r * light_factor;
	light_contrib.g = ctx.hit.color.g * light_factor;
	light_contrib.b = ctx.hit.color.b * light_factor;
	light_contrib.a = ctx.hit.color.a;
	if (light.color.r > 0 || light.color.g > 0 || light.color.b > 0)
		mixed_color = color_mix(light_contrib, light.color, 0.1);
	else
		mixed_color = light_contrib;
	return (color_add(ctx.current_color, mixed_color));
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
