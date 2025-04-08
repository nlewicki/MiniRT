/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 13:05:20 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/08 12:17:00 by nlewicki         ###   ########.fr       */
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

double parse_double(char *str, double min, double max, int *error)
{
	char *endptr;
	double value;

	errno = 0;
	value = strtod(str, &endptr);
	if (errno == ERANGE || *endptr != '\0' || value < min || value > max)
	{
		*error = 1;
		return (0);
	}
	return (value);
}

t_color	parse_color(char *str, int *error)
{
	char	**rgb;
	t_color	color;

	rgb = ft_split(str, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2])
	{
		*error = 1;
		return ((t_color){0, 0, 0});
	}
	color.r = ft_atoi(rgb[0]);
	color.g = ft_atoi(rgb[1]);
	color.b = ft_atoi(rgb[2]);
	if (color.r < 0 || color.r > 255 || color.g < 0 || color.g > 255
		|| color.b < 0 || color.b > 255)
	{
		*error = 1;
		return ((t_color){0, 0, 0});
	}
	ft_free_split(rgb);
	return (color);
}

t_vec3 parse_position(char *str, int *error)
{
	char **coords;
	t_vec3 vec;

	coords = ft_split(str, ',');
	if (!coords || !coords[0] || !coords[1] || !coords[2])
	{
		*error = 1;
		ft_free_split(coords);
		return ((t_vec3){0, 0, 0});
	}
	vec.x = parse_double(coords[0], -INFINITY, INFINITY, error);
	vec.y = parse_double(coords[1], -INFINITY, INFINITY, error);
	vec.z = parse_double(coords[2], -INFINITY, INFINITY, error);
	ft_free_split(coords);
	if (*error)
		return ((t_vec3){0, 0, 0});
	return (vec);
}

t_vec3 parse_orientation(char *str, int *error)
{
	char **coords;
	t_vec3 vec;
	double magnitude;

	coords = ft_split(str, ',');
	if (!coords || !coords[0] || !coords[1] || !coords[2])
	{
		*error = 1;
		ft_free_split(coords);
		return ((t_vec3){0, 0, 0});
	}
	vec.x = parse_double(coords[0], -1.0, 1.0, error);
	vec.y = parse_double(coords[1], -1.0, 1.0, error);
	vec.z = parse_double(coords[2], -1.0, 1.0, error);
	if (*error)
	{
		ft_free_split(coords);
		return ((t_vec3){0, 0, 0});
	}
	magnitude = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (magnitude < 0.99 || magnitude > 1.01) // Allow small float error
		*error = 1;
	ft_free_split(coords);
	if (*error)
		return ((t_vec3){0, 0, 0});
	return (vec);
}

void	exit_error(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

void parse_ambient(char **tokens, t_scene *scene)
{
	int error = 0;
	if (scene->ambient.is_set)
		exit_error("Ambient light already defined");
	if (!tokens[1] || !tokens[2] || tokens[3]) // Expect exactly 3 tokens: "A", ratio, color
		exit_error("Invalid ambient format");
	scene->ambient.ratio = parse_double(tokens[1], 0.0, 1.0, &error);
	scene->ambient.color = parse_color(tokens[2], &error);
	if (error)
		exit_error("Invalid ambient values");
	scene->ambient.is_set = 1;
	// debug
	printf("Ambient light set: ratio=%.2f, color=(%d, %d, %d)\n",
		scene->ambient.ratio, scene->ambient.color.r,
		scene->ambient.color.g, scene->ambient.color.b);
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
		// else if (ft_strcmp(tokens[0], "C") == 0)
		// 	parse_camera(tokens, scene);
		// else if (ft_strcmp(tokens[0], "L") == 0)
		// 	parse_light(tokens, scene);
		// else if (ft_strcmp(tokens[0], "sp") == 0)
		// 	parse_sphere(tokens, scene);
		// else if (ft_strcmp(tokens[0], "pl") == 0)
		// 	parse_plane(tokens, scene);
		// else if (ft_strcmp(tokens[0], "cy") == 0)
		// 	parse_cylinder(tokens, scene);
		else
		{
			free(line);
			ft_free_split(tokens);
			close(fd);
			exit_error("Unknown identifier");
		}
		free(line);
		ft_free_split(tokens);
	}
	close(fd);
	return (0);
}
