/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:25:10 by nicolewicki       #+#    #+#             */
/*   Updated: 2025/05/22 12:59:31 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	parse_limits(char *token, t_plane *plane, int *error)
{
	char	**limits;
	char	*trim;

	limits = ft_split(token, ',');
	if (limits && limits[0] && limits[1])
	{
		trim = ft_strtrim(limits[0], " \t\n\r");
		plane->limit_width = parse_double(trim, 0, INFINITY, error);
		free(trim);
		trim = ft_strtrim(limits[1], " \t\n\r");
		plane->limit_height = parse_double(trim, 0, INFINITY, error);
		free(trim);
	}
	ft_free_split(limits);
}

static void	parse_param(char **tokens, t_plane *plane)
{
	int	error;

	error = 0;
	plane->position = parse_position(tokens[1], &error);
	if (error)
		exit_error("Invalid plane position");
	plane->orientation = parse_orientation(tokens[2], &error);
	if (error)
		exit_error("Invalid plane orientation");
	plane->color = parse_color(tokens[3], &error);
	if (error)
		exit_error("Invalid plane color");
	plane->limit_width = 0;
	plane->limit_height = 0;
	plane->checker = false;
	if (tokens[4])
	{
		parse_limits(tokens[4], plane, &error);
		if (error)
			exit_error("Invalid plane limits");
	}
}

static void	add_plane_to_scene(t_scene *scene, t_plane plane)
{
	t_plane	*new_planes;

	new_planes = ft_realloc(scene->planes,
			sizeof(t_plane) * (scene->plane_count + 1));
	if (!new_planes)
		exit_error("Memory allocation failed for planes");
	scene->planes = new_planes;
	scene->planes[scene->plane_count] = plane;
	scene->plane_count++;
	scene->planes->reflection = REFLECTION;
}

void	parse_plane(char **tokens, t_scene *scene)
{
	t_plane	plane;

	if (!tokens[1] || !tokens[2] || !tokens[3])
		exit_error("Invalid plane format");
	parse_param(tokens, &plane);
	add_plane_to_scene(scene, plane);
	printf("Plane added: position=(%.2f, %.2f, %.2f),"
		"orientation=(%.2f, %.2f, %.2f), "
		"color=(%d, %d, %d), "
		"limit_width=%.2f, limit_height=%.2f\n",
		plane.position.x, plane.position.y, plane.position.z,
		plane.orientation.x, plane.orientation.y, plane.orientation.z,
		plane.color.r, plane.color.g, plane.color.b,
		plane.limit_width, plane.limit_height);
	printf("\n");
}
