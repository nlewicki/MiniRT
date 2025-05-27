/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cylinder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:25:06 by nicolewicki       #+#    #+#             */
/*   Updated: 2025/05/27 13:35:07 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	parse_param(char **tokens, t_cylinder *cylinder)
{
	int	error;

	error = 0;
	cylinder->position = parse_position(tokens[1], &error);
	if (error)
		exit_error("Invalid cylinder position");
	cylinder->orientation = parse_orientation(tokens[2], &error);
	if (error)
		exit_error("Invalid cylinder orientation");
	cylinder->diameter = parse_double(tokens[3], 0.0, INFINITY, &error);
	if (error)
		exit_error("Invalid cylinder diameter");
	cylinder->height = parse_double(tokens[4], 0.0, INFINITY, &error);
	if (error)
		exit_error("Invalid cylinder height");
	cylinder->color = parse_color(tokens[5], &error);
	if (error)
		exit_error("Invalid cylinder color");
	cylinder->checker = false;
}

static void	add_cylinder_to_scene(t_scene *scene, t_cylinder cylinder)
{
	t_cylinder	*new_cylinders;

	new_cylinders = ft_realloc(scene->cylinders,
			sizeof(t_cylinder) * (scene->cylinder_count + 1));
	if (!new_cylinders)
		exit_error("Memory allocation failed for cylinders");
	scene->cylinders = new_cylinders;
	scene->cylinders[scene->cylinder_count] = cylinder;
	scene->cylinder_count++;
	scene->cylinders->reflection = REFLECTION;
}

void	parse_cylinder(char **tokens, t_scene *scene)
{
	t_cylinder	cylinder;

	if (!tokens[1] || !tokens[2] || !tokens[3]
		|| !tokens[4] || !tokens[5] || tokens[6])
		exit_error("Invalid cylinder format");
	parse_param(tokens, &cylinder);
	add_cylinder_to_scene(scene, cylinder);
	printf("Cylinder added: position=(%.2f, %.2f, %.2f),"
		"orientation=(%.2f, %.2f, %.2f), "
		"diameter=%.2f, height=%.2f, color=(%d, %d, %d)\n",
		cylinder.position.x, cylinder.position.y, cylinder.position.z,
		cylinder.orientation.x, cylinder.orientation.y, cylinder.orientation.z,
		cylinder.diameter, cylinder.height,
		cylinder.color.r, cylinder.color.g, cylinder.color.b);
	printf("\n");
}
