/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 13:05:20 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/27 15:18:15 by lkubler          ###   ########.fr       */
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

static void	check_filename(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 3 || ft_strcmp(filename + len - 3, ".rt") != 0)
		exit_error("File must have .rt extension");
}

int	parse_rt_file(char *filename, t_scene *scene)
{
	int		fd;

	check_filename(filename);
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_error("Failed to open file");
	read_and_parse_lines(fd, scene);
	close(fd);
	return (0);
}
