/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 13:05:20 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/16 12:04:22 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	parse_identifier(char **tokens, t_scene *scene)
{
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
	else if (ft_strcmp(tokens[0], "co") == 0)
		parse_cone(tokens, scene);
	else
	{
		ft_free_split(tokens);
		exit_error("Unknown identifier");
	}
}

static void	process_line(char *line, t_scene *scene)
{
	char	**tokens;

	if (!line[0] || line[0] == '\n')
		return ;
	tokens = ft_split_whitespace(line);
	if (!tokens || !tokens[0])
	{
		free(line);
		ft_free_split(tokens);
		exit_error("Invalid line format");
	}
	parse_identifier(tokens, scene);
	ft_free_split(tokens);
}

static void	read_and_parse_lines(int fd, t_scene *scene)
{
	char	*line;

	line = get_next_line(fd);
	while (line != NULL)
	{
		process_line(line, scene);
		free(line);
		line = get_next_line(fd);
	}
}

int	parse_rt_file(char *filename, t_scene *scene)
{
	int		fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error("Failed to open file");
	read_and_parse_lines(fd, scene);
	close(fd);
	return (0);
}
