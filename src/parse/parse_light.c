/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolewicki <nicolewicki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:28:11 by nicolewicki       #+#    #+#             */
/*   Updated: 2025/04/09 15:28:18 by nicolewicki      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void parse_light(char **tokens, t_scene *scene)
{
	int error = 0;
	t_light *new_lights;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4]) // Expect exactly 4 tokens: "L", position, brightness, color
		exit_error("Invalid light format");

	t_light light;
	light.position = parse_position(tokens[1], &error); // vllt auch error check nach jedem einbauen?
	light.brightness = parse_double(tokens[2], 0.0, 1.0, &error);
	light.color = parse_color(tokens[3], &error);
	if (error)
		exit_error("Invalid light values");
	new_lights = ft_realloc(scene->lights, sizeof(t_light) * (scene->light_count + 1));
	if (!new_lights)
		exit_error("Memory allocation failed for lights");
	scene->lights = new_lights;
	scene->lights[scene->light_count] = light;
	scene->light_count++;
	// Debug print
	printf("Light added: position=(%.2f, %.2f, %.2f), brightness=%.2f, color=(%d, %d, %d)\n",
		light.position.x, light.position.y, light.position.z,
		light.brightness, light.color.r, light.color.g, light.color.b);
	write(1, "\n", 2);
}