/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:11:42 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/27 16:54:00 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return ;
	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
}

void	exit_error(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

void	free_scene(t_scene *scene)
{
	int	i;

	if (scene->lights)
		free(scene->lights);
	if (scene->spheres)
		free(scene->spheres);
	if (scene->planes)
		free(scene->planes);
	if (scene->cylinders)
		free(scene->cylinders);
	if (scene->cones)
		free(scene->cones);
	if (scene->objects)
	{
		i = 0;
		while (i < scene->object_count)
		{
			free(scene->objects[i].data);
			i++;
		}
		free(scene->objects);
	}
}

t_color	compute_ambient_only(t_miniRT *mini, t_hit hit)
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

// Calculate colored light contribution
t_color	calculate_colored_light(t_hit hit, t_light light,
	double light_factor)
{
	t_color	diffuse_contrib;
	t_color	light_tint;
	t_color	final_contrib;

	diffuse_contrib.r = hit.color.r * light_factor;
	diffuse_contrib.g = hit.color.g * light_factor;
	diffuse_contrib.b = hit.color.b * light_factor;
	diffuse_contrib.a = hit.color.a;
	light_tint.r = light.color.r * light_factor * 0.4;
	light_tint.g = light.color.g * light_factor * 0.4;
	light_tint.b = light.color.b * light_factor * 0.4;
	light_tint.a = 255;
	final_contrib = color_add(diffuse_contrib, light_tint);
	return (final_contrib);
}
