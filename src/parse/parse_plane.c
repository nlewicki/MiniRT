/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:25:10 by nicolewicki       #+#    #+#             */
/*   Updated: 2025/05/06 13:24:22 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	parse_plane(char **tokens, t_scene *scene)
{
	int		error;
	t_plane	*new_planes;
	t_plane	plane;

	error = 0;
	if (!tokens[1] || !tokens[2] || !tokens[3])
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
	plane.material_link = NULL;
	plane.limit_width = 0;
	plane.limit_height = 0;
	plane.checker = false;
	if (tokens[4]) {
		plane.material_link = ft_strdup(tokens[4]);
		if (tokens[5]) {
			char **limits = ft_split(tokens[5], ',');
			if (limits && limits[0] && limits[1])
			{
				char *trim = ft_strtrim(limits[0], " \t\n\r");
				plane.limit_width = parse_double(trim, 0, INFINITY, &error);
				free(trim);
				trim = ft_strtrim(limits[1], " \t\n\r");
				plane.limit_height = parse_double(trim, 0, INFINITY, &error);
				free(trim);
			}
			ft_free_split(limits);
		}
	}
	new_planes = ft_realloc(scene->planes,
			sizeof(t_plane) * (scene->plane_count + 1));
	if (!new_planes)
		exit_error("Memory allocation failed for planes");
	scene->planes = new_planes;
	scene->planes[scene->plane_count] = plane;
	scene->plane_count++;
	scene->planes->reflection = REFLECTION;
	scene->planes->shine = SHINE;
	scene->planes->ks = KS;
	// Debug print
	printf("Plane added: position=(%.2f, %.2f, %.2f), orientation=(%.2f, %.2f, %.2f), color=(%d, %d, %d), material_link=%s, limit_width=%.2f, limit_height=%.2f\n",
		plane.position.x, plane.position.y, plane.position.z,
		plane.orientation.x, plane.orientation.y, plane.orientation.z,
		plane.color.r, plane.color.g, plane.color.b, plane.material_link, plane.limit_width, plane.limit_height);
	printf("\n");
}
