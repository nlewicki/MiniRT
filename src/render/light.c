/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 17:40:34 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/22 14:04:08 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

// Modified to accept a skip_object parameter
static double	compute_shadow_factor(t_miniRT *mini, t_vec3 point,
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

static t_color	add_light_contribution(t_miniRT *mini, t_hit hit,
	t_light light, t_object *skip_object,
	t_color current_color)
{
	t_vec3	light_dir;
	double	shadow;
	double	diffuse;
	t_color	light_contrib;

	light_dir = vec_normalize(vec_sub(light.position, hit.point));
	shadow = compute_shadow_factor(mini, hit.point, light, skip_object);
	shadow = pow(shadow, 0.7);
	if (shadow <= 0.0)
		return (current_color);
	diffuse = fmax(0.0, vec_skal(hit.normal, light_dir));
	light_contrib = color_scale(hit.color, diffuse * light.brightness * shadow);
	light_contrib = color_mix(light_contrib, light.color, 0.2);
	return (color_add(current_color, light_contrib));
}

static t_color	compute_all_lights(t_miniRT *mini, t_hit hit,
	t_object *skip_object, t_color ambient_color)
{
	int		i;
	t_color	final_color;

	final_color = ambient_color;
	i = 0;
	while (i < mini->scene.light_count)
	{
		final_color = add_light_contribution(mini, hit,
				mini->scene.lights[i], skip_object, final_color);
		i++;
	}
	return (color_clamp(final_color));
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

// New function that can skip an object during shadow calculations
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
