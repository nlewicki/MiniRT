/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_camera.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolewicki <nicolewicki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:24:10 by nicolewicki       #+#    #+#             */
/*   Updated: 2025/05/26 12:48:49 by nicolewicki      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static t_vec3	zero_vec(t_vec3 pos, int *error)
{
	t_vec3	v;

	v = (t_vec3)
	{
		-pos.x,
		-pos.y,
		-pos.z
	};
	return (normalize_vector(v, error));
}

void	parse_camera(char **tokens, t_scene *scene)
{
	int	error;

	error = 0;
	if (scene->camera.is_set)
		exit_error("Camera already defined");
	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		exit_error("Invalid camera format");
	scene->camera.position = parse_position(tokens[1], &error);
	if (!strcmp(tokens[2], "0,0,0"))
		scene->camera.orientation = zero_vec(scene->camera.position, &error);
	else
		scene->camera.orientation = parse_orientation(tokens[2], &error);
	scene->camera.fov = ft_atoi(tokens[3]);
	if (scene->camera.fov < 0 || scene->camera.fov > 180)
		error = 1;
	if (error)
		exit_error("Invalid camera values");
	scene->camera.is_set = 1;
	printf("Camera set: position=(%.2f, %.2f, %.2f),"
		"orientation=(%.2f, %.2f, %.2f), fov=%d\n",
		scene->camera.position.x, scene->camera.position.y,
		scene->camera.position.z, scene->camera.orientation.x,
		scene->camera.orientation.y, scene->camera.orientation.z,
		scene->camera.fov);
	write(1, "\n", 2);
}
