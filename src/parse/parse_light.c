/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:28:11 by nicolewicki       #+#    #+#             */
/*   Updated: 2025/05/12 13:28:57 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	add_light_to_scene(t_scene *scene, t_light light)
{
	t_light	*new_lights;

	new_lights = ft_realloc(scene->lights, sizeof(t_light) * (scene->light_count
				+ 1));
	if (!new_lights)
		exit_error("Memory allocation failed for lights");
	scene->lights = new_lights;
	scene->lights[scene->light_count] = light;
	scene->light_count++;
}

void	parse_light(char **tokens, t_scene *scene)
{
	t_light	light;
	int		error;

	error = 0;
	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		exit_error("Invalid light format");
	light.position = parse_position(tokens[1], &error);
	if (error)
		exit_error("Invalid light position");
	light.brightness = parse_double(tokens[2], 0.0, 1.0, &error);
	if (error)
		exit_error("Invalid light brightness");
	light.color = parse_color(tokens[3], &error);
	if (error)
		exit_error("Invalid light color");
	add_light_to_scene(scene, light);
	printf("Light added: position=(%.2f, %.2f, %.2f), brightness=%.2f, "
		"color=(%d, %d, %d)\n",
		light.position.x, light.position.y, light.position.z,
		light.brightness, light.color.r, light.color.g, light.color.b);
	printf("\n");
}
