/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:25:10 by nicolewicki       #+#    #+#             */
/*   Updated: 2025/04/28 12:23:10 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	parse_plane(char **tokens, t_scene *scene)
{
	int		error;
	t_plane	*new_planes;
	t_plane	plane;

	error = 0;
	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		exit_error("Invalid plane format");
	plane.position = parse_position(tokens[1], &error);
	if (error)
		exit_error("Invalid plane position");
	plane.orientation = parse_orientation(tokens[2], &error);
	if (error)
		exit_error("Invalid plane orientation");
	plane.color = parse_color(tokens[3], &error);
	if (error)
		exit_error("Invalid plane color");
	new_planes = ft_realloc(scene->planes,
			sizeof(t_plane) * (scene->plane_count + 1));
	if (!new_planes)
		exit_error("Memory allocation failed for planes");
	scene->planes = new_planes;
	scene->planes[scene->plane_count] = plane;
	scene->plane_count++;
	// Debug print
	printf("Plane added: position=(%.2f, %.2f, %.2f), orientation=(%.2f, %.2f, %.2f), color=(%d, %d, %d)\n",
		plane.position.x, plane.position.y, plane.position.z,
		plane.orientation.x, plane.orientation.y, plane.orientation.z,
		plane.color.r, plane.color.g, plane.color.b);
	printf("\n");
}
