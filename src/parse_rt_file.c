/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolewicki <nicolewicki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 13:05:20 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/09 13:18:51 by nicolewicki      ###   ########.fr       */
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
	write(1, "\n", 2);
}

void parse_camera(char **tokens, t_scene *scene)
{
	int error = 0;
	if (scene->camera.is_set)
		exit_error("Camera already defined");
	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4]) // Expect exactly 4 tokens: "C", position, orientation, fov
		exit_error("Invalid camera format");
	scene->camera.position = parse_position(tokens[1], &error);
	scene->camera.orientation = parse_orientation(tokens[2], &error);
	scene->camera.fov = ft_atoi(tokens[3]);
	if (scene->camera.fov < 0 || scene->camera.fov > 180)
		error = 1;
	if (error)
		exit_error("Invalid camera values");
	scene->camera.is_set = 1;
	// debug
	printf("Camera set: position=(%.2f, %.2f, %.2f), orientation=(%.2f, %.2f, %.2f), fov=%d\n",
		scene->camera.position.x, scene->camera.position.y,
		scene->camera.position.z, scene->camera.orientation.x,
		scene->camera.orientation.y, scene->camera.orientation.z,
		scene->camera.fov);
	write(1, "\n", 2);
}

void parse_light(char **tokens, t_scene *scene)
{
	int error = 0;
	t_light *new_lights;

	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4]) // Expect exactly 4 tokens: "L", position, brightness, color
		exit_error("Invalid light format");

	t_light light;
	light.position = parse_position(tokens[1], &error); // vllt auch error check nach jedem einbauen?
	light.brightness = parse_double(tokens[2], 0.0, 1.0, &error);
	light.color = parse_color(tokens[3], &error);
	if (error)
		exit_error("Invalid light values");
	new_lights = ft_realloc(scene->lights, sizeof(t_light) * (scene->light_count + 1));
	if (!new_lights)
		exit_error("Memory allocation failed for lights");
	scene->lights = new_lights;
	scene->lights[scene->light_count] = light;
	scene->light_count++;
	// Debug print
	printf("Light added: position=(%.2f, %.2f, %.2f), brightness=%.2f, color=(%d, %d, %d)\n",
		light.position.x, light.position.y, light.position.z,
		light.brightness, light.color.r, light.color.g, light.color.b);
	write(1, "\n", 2);
}

void parse_sphere(char **tokens, t_scene *scene)
{
    int error = 0;
    t_sphere *new_sphere;

    if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4]) // Expect exactly 4 tokens: "sp", center, diameter, color
        exit_error("Invalid sphere format");

    t_sphere sphere;
    sphere.center = parse_position(tokens[1], &error);
    if (error)
        exit_error("Invalid sphere center");
    sphere.diameter = parse_double(tokens[2], 0.0, INFINITY, &error);
    if (error)
        exit_error("Invalid sphere diameter");
    sphere.color = parse_color(tokens[3], &error);
    if (error)
        exit_error("Invalid sphere color");

    new_sphere = ft_realloc(scene->spheres, sizeof(t_sphere) * (scene->sphere_count + 1));
    if (!new_sphere)
        exit_error("Memory allocation failed for spheres");
    scene->spheres = new_sphere;
    scene->spheres[scene->sphere_count] = sphere;
    scene->sphere_count++;

    // Debug print
    printf("Sphere added: center=(%.2f, %.2f, %.2f), diameter=%.2f, color=(%d, %d, %d)\n",
           sphere.center.x, sphere.center.y, sphere.center.z, sphere.diameter,
           sphere.color.r, sphere.color.g, sphere.color.b);
    printf("\n"); // Simpler than write
}

void parse_plane(char **tokens, t_scene *scene)
{
    int error = 0;
    t_plane *new_planes;

    if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4]) // Expect exactly 4 tokens: "pl", position, orientation, color
        exit_error("Invalid plane format");

    t_plane plane;
    plane.position = parse_position(tokens[1], &error);
    if (error)
        exit_error("Invalid plane position");
    plane.orientation = parse_orientation(tokens[2], &error);
    if (error)
        exit_error("Invalid plane orientation");
    plane.color = parse_color(tokens[3], &error);
    if (error)
        exit_error("Invalid plane color");

    new_planes = ft_realloc(scene->planes, sizeof(t_plane) * (scene->plane_count + 1));
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

void parse_cylinder(char **tokens, t_scene *scene)
{
    int error;
    t_cylinder *new_cylinders;
	t_cylinder cylinder;

	error = 0;
    if (!tokens[1] || !tokens[2] || !tokens[3] || !tokens[4] || !tokens[5] || tokens[6]) // Expect exactly 6 tokens: "cy", position, orientation, diameter, height, color
        exit_error("Invalid cylinder format");
    cylinder.position = parse_position(tokens[1], &error);
    if (error)
        exit_error("Invalid cylinder position");
    cylinder.orientation = parse_orientation(tokens[2], &error);
    if (error)
        exit_error("Invalid cylinder orientation");
    cylinder.diameter = parse_double(tokens[3], 0.0, INFINITY, &error);
    if (error)
        exit_error("Invalid cylinder diameter");
    cylinder.height = parse_double(tokens[4], 0.0, INFINITY, &error);
    if (error)
        exit_error("Invalid cylinder height");
    cylinder.color = parse_color(tokens[5], &error);
    if (error)
        exit_error("Invalid cylinder color");
    new_cylinders = ft_realloc(scene->cylinders, sizeof(t_cylinder) * (scene->cylinder_count + 1));
    if (!new_cylinders)
        exit_error("Memory allocation failed for cylinders");
    scene->cylinders = new_cylinders;
    scene->cylinders[scene->cylinder_count] = cylinder;
    scene->cylinder_count++;

    // Debug print
    printf("Cylinder added: position=(%.2f, %.2f, %.2f), orientation=(%.2f, %.2f, %.2f), diameter=%.2f, height=%.2f, color=(%d, %d, %d)\n",
           cylinder.position.x, cylinder.position.y, cylinder.position.z,
           cylinder.orientation.x, cylinder.orientation.y, cylinder.orientation.z,
           cylinder.diameter, cylinder.height,
           cylinder.color.r, cylinder.color.g, cylinder.color.b);
    printf("\n");
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
