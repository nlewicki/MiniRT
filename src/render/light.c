/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leokubler <leokubler@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:40:34 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/26 10:16:25 by leokubler        ###   ########.fr       */
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

static t_color	add_light_contribution(t_light_context ctx, t_light light)
{
	double	light_factor;
	t_color	light_contrib;
	t_color	tint;

	light_factor = calculate_light_factors(ctx, light);
	if (light_factor < 0.0)
		return (ctx.current_color);
	light_contrib = color_scale(ctx.hit.color, light_factor);
	if (light.color.r > 0 || light.color.g > 0 || light.color.b > 0)
	{
		tint = color_scale(light.color, light_factor * 0.2);
		light_contrib = color_add(light_contrib, tint);
	}
	return (color_add(ctx.current_color, light_contrib));
}

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

static t_color	compute_default_lighting(t_miniRT *mini, t_hit hit)
{
	t_vec3	default_light_dir;
	double	diffuse;
	t_color	light_contrib;
	t_color	final_color;

	default_light_dir = (t_vec3){0, -1, 0};
	diffuse = fmax(0.0, vec_skal(hit.normal, vec_neg(default_light_dir)));
	light_contrib = color_scale(hit.color, diffuse * 0.7);
	final_color = color_add(color_scale(mini->scene.ambient.color,
				mini->scene.ambient.ratio), light_contrib);
	return (color_clamp(final_color));
}

//function to skip an object during shadow calculations
t_color	compute_lighting_skip_object(t_miniRT *mini, t_hit hit,
	t_object *skip_object)
{
	t_color	final_color;

	final_color = color_scale(mini->scene.ambient.color,
			mini->scene.ambient.ratio);
	if (mini->scene.light_count == 0)
		return (compute_default_lighting(mini, hit));
	return (compute_all_lights(mini, hit, skip_object, final_color));
}
