/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolewicki <nicolewicki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 13:05:20 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/09 15:41:27 by nicolewicki      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void 	ft_free_split(char **split)
{
	int	i;

	if (!split)
		return;
	i = 0;
	while (split[i])
		free(split[i++]);
		i++;
	free(split);
}

void	exit_error(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

void free_scene(t_scene *scene)
{
    if (scene->lights)
        free(scene->lights);
    if (scene->spheres)
        free(scene->spheres);
    if (scene->planes)
        free(scene->planes);
    if (scene->cylinders)
        free(scene->cylinders);
}

int parse_rt_file(char *filename, t_scene *scene)
{
	printf("Parsing file: %s\n", filename);
	int fd;
	char *line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error("Failed to open file");

	while ((line = get_next_line(fd)) != NULL)
	{
		char **tokens;

		if (!line[0] || line[0] == '\n') // Skip empty lines
		{
			free(line);
			continue;
		}
		tokens = ft_split(line, ' ');
		if (!tokens || !tokens[0])
		{
			free(line);
			ft_free_split(tokens);
			close(fd);
			exit_error("Invalid line format");
		}
		if (ft_strcmp(tokens[0], "A") == 0)
			parse_ambient(tokens, scene);
		else if (ft_strcmp(tokens[0], "C") == 0)
			parse_camera(tokens, scene);
		else if (ft_strcmp(tokens[0], "L") == 0)
			parse_light(tokens, scene);
		else if (ft_strcmp(tokens[0], "sp") == 0)
			parse_sphere(tokens, scene);
		else if (ft_strcmp(tokens[0], "pl") == 0)
			parse_plane(tokens, scene);
		else if (ft_strcmp(tokens[0], "cy") == 0)
			parse_cylinder(tokens, scene);
		else
		{
			free(line);
			ft_free_split(tokens);
			close(fd);
			free_scene(scene);
			exit_error("Unknown identifier");
		}
		free(line);
		ft_free_split(tokens);
	}
	close(fd);
	return (0);
}
