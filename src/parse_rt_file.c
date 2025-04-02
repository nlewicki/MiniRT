/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_rt_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 13:05:20 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/02 13:43:32 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void parse_ambient(char *line, t_ambient *ambient)
{
	// // Parse ambient lighting
	// // Format: A ratio color
	// // Example: A 0.2 255,255,255

	// // Check if ambient lighting is already set
	// if (ambient->is_set)
	// 	return (printf("Ambient lighting is already set"));

	// // Parse ratio
	// ambient->ratio = ft_atof(line + 2);

	// // Parse color
	// ambient->color.r = ft_atoi(line + 4);
	// ambient->color.g = ft_atoi(line + 7);
	// ambient->color.b = ft_atoi(line + 10);

	// // Set is_set to true
	// ambient->is_set = 1;
	// // (void)ambient;
	// // (void)line;
	printf("Parsing ambient\n");
}

static void parse_camera(char *line, t_camera *camera)
{
	// // Parse camera
	// // Format: C position direction fov
	// // Example: C 0,0,0 0,0,1 90

	// // Parse position
	// camera->position.x = ft_atof(line + 2);
	// camera->position.y = ft_atof(line + 4);
	// camera->position.z = ft_atof(line + 6);

	// // Parse direction
	// camera->direction.x = ft_atof(line + 8);
	// camera->direction.y = ft_atof(line + 10);
	// camera->direction.z = ft_atof(line + 12);

	// // Parse fov
	// camera->fov = ft_atoi(line + 14);
	(void)camera;
	(void)line;
	printf("Parsing camera\n");
}

static void parse_light(char *line, t_scene *scene)
{
	// // Parse light
	// // Format: L position ratio color
	// // Example: L 0,0,0 0.6 255,255,255

	// t_light *light;

	// light = malloc(sizeof(t_light));
	// if (!light)
	// 	return (error_message("Malloc failed"));

	// // Parse position
	// light->position.x = ft_atof(line + 2);
	// light->position.y = ft_atof(line + 4);
	// light->position.z = ft_atof(line + 6);

	// // Parse ratio
	// light->ratio = ft_atof(line + 8);

	// // Parse color
	// light->color.r = ft_atoi(line + 10);
	// light->color.g = ft_atoi(line + 13);
	// light->color.b = ft_atoi(line + 16);

	// // Add light to scene
	// ft_lstadd_back(&scene->lights, ft_lstnew(light));
	(void)scene;
	(void)line;
	printf("Parsing light\n");
}

static void parse_sphere(char *line, t_scene *scene)
{
	// // Parse sphere
	// // Format: sp center diameter color
	// // Example: sp 0,0,0 1 255,255,255

	// t_sphere *sphere;

	// sphere = malloc(sizeof(t_sphere));
	// if (!sphere)
	// 	return (error_message("Malloc failed"));

	// // Parse center
	// sphere->center.x = ft_atof(line + 3);
	// sphere->center.y = ft_atof(line + 5);
	// sphere->center.z = ft_atof(line + 7);

	// // Parse diameter
	// sphere->diameter = ft_atof(line + 9);

	// // Parse color
	// sphere->color.r = ft_atoi(line + 11);
	// sphere->color.g = ft_atoi(line + 14);
	// sphere->color.b = ft_atoi(line + 17);

	// // Add sphere to scene
	// ft_lstadd_back(&scene->spheres, ft_lstnew(sphere));
	(void)scene;
	(void)line;
	printf("Parsing sphere\n");
}

static void parse_plane(char *line, t_scene *scene)
{
	// // Parse plane
	// // Format: pl position orientation color
	// // Example: pl 0,0,0 0,0,1 255,255,255

	// t_plane *plane;

	// plane = malloc(sizeof(t_plane));
	// if (!plane)
	// 	return (error_message("Malloc failed"));

	// // Parse position
	// plane->position.x = ft_atof(line + 3);
	// plane->position.y = ft_atof(line + 5);
	// plane->position.z = ft_atof(line + 7);

	// // Parse orientation
	// plane->orientation.x = ft_atof(line + 9);
	// plane->orientation.y = ft_atof(line + 11);
	// plane->orientation.z = ft_atof(line + 13);

	// // Parse color
	// plane->color.r = ft_atoi(line + 15);
	// plane->color.g = ft_atoi(line + 18);
	// plane->color.b = ft_atoi(line + 21);

	// // Add plane to scene
	// ft_lstadd_back(&scene->planes, ft_lstnew(plane));
	(void)scene;
	(void)line;
	printf("Parsing plane\n");
}

static void parse_cylinder(char *line, t_scene *scene)
{
	// // Parse cylinder
	// // Format: cy position orientation diameter color
	// // Example: cy 0,0,0 0,0,1 1 255,255,255

	// t_cylinder *cylinder;

	// cylinder = malloc(sizeof(t_cylinder));
	// if (!cylinder)
	// 	return (error_message("Malloc failed"));

	// // Parse position
	// cylinder->position.x = ft_atof(line + 3);
	// cylinder->position.y = ft_atof(line + 5);
	// cylinder->position.z = ft_atof(line + 7);

	// // Parse orientation
	// cylinder->orientation.x = ft_atof(line + 9);
	// cylinder->orientation.y = ft_atof(line + 11);
	// cylinder->orientation.z = ft_atof(line + 13);

	// // Parse diameter
	// cylinder->diameter = ft_atof(line + 15);

	// // Parse color
	// cylinder->color.r = ft_atoi(line + 17);
	// cylinder->color.g = ft_atoi(line + 20);
	// cylinder->color.b = ft_atoi(line + 23);

	// // Add cylinder to scene
	// ft_lstadd_back(&scene->cylinders, ft_lstnew(cylinder));
	(void)scene;
	(void)line;
	printf("Parsing cylinder\n");
}

int parse_rt_file(char *filename, t_scene *scene)
{
    int fd;
    char *line;

    // Initialize scene with default values

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
		else if (line[0] == 'p' && line[1] == 'l' && line[2] == ' ')
			parse_plane(line, scene);
		else if (line[0] == 'c' && line[1] == 'y' && line[2] == ' ')
			parse_cylinder(line, scene);
        else
            return (ft_putendl_fd("Unknown element in scene file", 2), 1);
        free(line);
    }

    close(fd);

    // Validate scene (check if required elements exist)

    return (0); // Success
}
