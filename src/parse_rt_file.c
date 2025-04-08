/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 13:05:20 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/07 13:40:59 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	ft_free(char **result)
{
	int i;
	if (result == NULL)
		return ;
	i = 0;
	while (result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
}

static int parse_vector(char *str, t_vec3 *vec)
{
	char **values = ft_split(str, ',');
	if (!values || !values[0] || !values[1] || !values[2])
	{
		ft_free(values);
		return (0);
	}

	vec->x = ft_atof(values[0]);
	vec->y = ft_atof(values[1]);
	vec->z = ft_atof(values[2]);

	ft_free(values);
	return (1);
}

// Parse a color from string like "255,128,0"
static int parse_color(char *str, t_color *color)
{
	char **values = ft_split(str, ',');
	if (!values || !values[0] || !values[1] || !values[2])
	{
		ft_free(values);
		return (0);
	}

	color->r = ft_atoi(values[0]);
	color->g = ft_atoi(values[1]);
	color->b = ft_atoi(values[2]);

	// Validate color values
	if (color->r < 0 || color->r > 255 ||
		color->g < 0 || color->g > 255 ||
		color->b < 0 || color->b > 255)
	{
		ft_free(values);
		return (0);
	}

	ft_free(values);
	return (1);
}

// Parse a floating point value with range validation
static int parse_ratio(char *str, double *ratio, double min, double max)
{
	*ratio = ft_atof(str);
	return (*ratio >= min && *ratio <= max);
}

// General error handler for parsing errors
static void parsing_error(char **values, const char *message)
{
	ft_putendl_fd((char *)message, 2);
	ft_free(values); // -1 means free all values in array
	exit(1);
}

// Improved parsing functions using the helpers

static void parse_ambient(char *line, t_ambient *ambient)
{
	char **values;

	if (ambient->is_set)
		parsing_error(NULL, "Ambient lighting is already set");

	values = ft_split(line, ' ');
	if (!values || !values[1] || !values[2])
		parsing_error(values, "Invalid ambient lighting format");

	// Parse ratio
	if (!parse_ratio(values[1], &ambient->ratio, 0.0, 1.0))
		parsing_error(values, "Invalid ambient ratio (must be between 0 and 1)");

	// Parse color
	if (!parse_color(values[2], &ambient->color))
		parsing_error(values, "Invalid ambient color format");

	ambient->is_set = 1;
	ft_free(values);
	printf("Parsing ambient\n");
}

static void parse_camera(char *line, t_camera *camera)
{
	char **values;

	if (camera->is_set)
		parsing_error(NULL, "Camera is already set");

	values = ft_split(line, ' ');
	if (!values || !values[1] || !values[2] || !values[3])
		parsing_error(values, "Invalid camera format");

	// Parse position and direction
	if (!parse_vector(values[1], &camera->position))
		parsing_error(values, "Invalid camera position format");

	if (!parse_vector(values[2], &camera->orientation))
		parsing_error(values, "Invalid camera direction format");

	// Parse FOV
	camera->fov = ft_atoi(values[3]);
	if (camera->fov < 0 || camera->fov > 180)
		parsing_error(values, "Invalid FOV (must be between 0 and 180)");

	camera->is_set = 1;
	ft_free(values);
	printf("Parsing camera\n");
}

static void parse_light(char *line, t_scene *scene)
{
	char **values;
	t_light *new_light;

	values = ft_split(line, ' ');
	if (!values || !values[1] || !values[2] || !values[3])
		parsing_error(values, "Invalid light format");
	new_light = (t_light *)malloc(sizeof(t_light));
	if (!new_light)
		parsing_error(values, "Memory allocation failed for light");
	if (!parse_vector(values[1], &new_light->position))
	{
		free(new_light);
		parsing_error(values, "Invalid light position format");
	}

	if (!parse_ratio(values[2], &new_light->brightness, 0.0, 1.0))
	{
		free(new_light);
		parsing_error(values, "Invalid light brightness (must be between 0 and 1)");
	}

	if (!parse_color(values[3], &new_light->color))
	{
		free(new_light);
		parsing_error(values, "Invalid light color format");
	}
	t_light *temp = realloc(scene->lights, sizeof(t_light) * (scene->light_count + 1));
	if (!temp)
	{
		free(new_light);
		parsing_error(values, "Memory allocation failed when extending lights array");
	}

	scene->lights = temp;
	scene->lights[scene->light_count] = *new_light;
	scene->light_count++;
	free(new_light);

	ft_free(values);
	printf("Parsing light - added light #%d\n", scene->light_count);
}

// You can implement similar parsing functions for sphere, plane, and cylinder
static void parse_sphere(char *line, t_scene *scene)
{
	(void)scene; // Assuming you will add the sphere to the scene later
	char **values;
	t_sphere *sphere;

	values = ft_split(line, ' ');
	if (!values || !values[1] || !values[2] || !values[3])
		parsing_error(values, "Invalid sphere format");
	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		parsing_error(values, "Memory allocation failed for sphere");
	if (!parse_vector(values[1], &sphere->center))
	{
		free(sphere);
		parsing_error(values, "Invalid sphere center format");
	}
	sphere->diameter = ft_atof(values[2]);
	if (sphere->diameter <= 0)
	{
		free(sphere);
		parsing_error(values, "Invalid sphere diameter (must be positive)");
	}
	if (!parse_color(values[3], &sphere->color))
	{
		free(sphere);
		parsing_error(values, "Invalid sphere color format");
	}

	// Add sphere to scene (assuming you have a function or structure for this)
	ft_lstadd_back(&scene->spheres, ft_lstnew(sphere));

	ft_free(values);
	printf("Parsing sphere\n");
}

int print_struct(t_scene *scene)
{
	write(1, "\nScene:\n", 9);
	// Print the parsed scene for debugging
	printf("Ambient: %d\n", scene->ambient.is_set);
	printf("Camera: %d\n", scene->camera.is_set);
	// Print lights, spheres, planes, and cylinders
	printf("Ambient ratio: %f\n", scene->ambient.ratio);
	printf("Ambient color: %d,%d,%d\n", scene->ambient.color.r, scene->ambient.color.g, scene->ambient.color.b);

	// Print camera details
	printf("Camera position: %f,%f,%f\n", scene->camera.position.x, scene->camera.position.y, scene->camera.position.z);
	printf("Camera orientation: %f,%f,%f\n", scene->camera.orientation.x, scene->camera.orientation.y, scene->camera.orientation.z);
	printf("Camera fov: %d\n", scene->camera.fov);

	// Print lights
	printf("\nLights (%d):\n", scene->light_count);
	for (int i = 0; i < scene->light_count; i++)
	{
		printf("Light %d:\n", i + 1);
		printf("  Position: %f,%f,%f\n", scene->lights[i].position.x, scene->lights[i].position.y, scene->lights[i].position.z);
		printf("  Brightness: %f\n", scene->lights[i].brightness);
		printf("  Color: %d,%d,%d\n", scene->lights[i].color.r, scene->lights[i].color.g, scene->lights[i].color.b);
	}

	return (0);
}

// Modified parse_rt_file to use the improved functions
int parse_rt_file(char *filename, t_scene *scene)
{
	int fd;
	char *line;

	// Initialize scene with default values
	scene->light_count = 0;
	scene->lights = NULL;
	scene->ambient.is_set = 0;
	scene->camera.is_set = 0;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ft_putendl_fd("Cannot open file", 2), 1);

	// Read line by line
	while ((line = get_next_line(fd)))
	{
		// Skip empty lines or comments
		if (!*line || *line == '#')
		{
			free(line);
			continue;
		}

		// Parse based on identifier
		if (line[0] == 'A' && line[1] == ' ')
			parse_ambient(line, &scene->ambient);
		else if (line[0] == 'C' && line[1] == ' ')
			parse_camera(line, &scene->camera);
		else if (line[0] == 'L' && line[1] == ' ')
			parse_light(line, scene);
		else if (line[0] == 's' && line[1] == 'p' && line[2] == ' ')
			parse_sphere(line, scene);
		// else if (line[0] == 'p' && line[1] == 'l' && line[2] == ' ')
		// 	parse_plane(line, scene);
		// else if (line[0] == 'c' && line[1] == 'y' && line[2] == ' ')
		// 	parse_cylinder(line, scene);
		else
			return (ft_putendl_fd("Unknown element in scene file", 2), 1);
		free(line);
	}

	close(fd);
	print_struct(scene);

	// Validate scene (check if required elements exist)
	if (!scene->ambient.is_set || !scene->camera.is_set)
		return (ft_putendl_fd("Missing required scene elements", 2), 1);

	return (0); // Success
}
