/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_object.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:23:41 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/22 17:10:30 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

//double hit_sphere(t_object *obj, const t_ray ray, t_hit *hit_info)
//{
//	(void)obj; (void)ray; (void)hit_info;
//	return (-1.0); // Placeholder: -1 means no hit
//}

//double hit_plane(t_object *obj, const t_ray ray, t_hit *hit_info)
//{
//	(void)obj; (void)ray; (void)hit_info;
//	return (-1.0); // Placeholder
//}

double hit_cylinder(t_object *obj, const t_ray ray, t_hit *hit_info)
{
	(void)obj; (void)ray; (void)hit_info;
	return (-1.0); // Placeholder
}

void convert_spheres(t_scene *scene, int *index)
{
	int i;

	for (i = 0; i < scene->sphere_count; i++)
	{
		t_sphere *sphere_data = malloc(sizeof(t_sphere));
		if (!sphere_data)
			exit_error("Memory allocation failed for sphere data");
		*sphere_data = scene->spheres[i]; // Copy sphere data
		scene->objects[*index].type = SPHERE;
		scene->objects[*index].data = sphere_data;
		scene->objects[*index].color = sphere_data->color;
		scene->objects[*index].hit = hit_sphere;
		(*index)++;
	}
}

void convert_planes(t_scene *scene, int *index)
{
	int i;

	for (i = 0; i < scene->plane_count; i++)
	{
		t_plane *plane_data = malloc(sizeof(t_plane));
		if (!plane_data)
			exit_error("Memory allocation failed for plane data");
		*plane_data = scene->planes[i];
		scene->objects[*index].type = PLANE;
		scene->objects[*index].data = plane_data;
		scene->objects[*index].color = plane_data->color;
		scene->objects[*index].hit = hit_plane;
		(*index)++;
	}
}

void convert_cylinders(t_scene *scene, int *index)
{
	int i;

	for (i = 0; i < scene->cylinder_count; i++)
	{
		t_cylinder *cylinder_data = malloc(sizeof(t_cylinder));
		if (!cylinder_data)
			exit_error("Memory allocation failed for cylinder data");
		*cylinder_data = scene->cylinders[i];
		scene->objects[*index].type = CYLINDER;
		scene->objects[*index].data = cylinder_data;
		scene->objects[*index].color = cylinder_data->color;
		scene->objects[*index].hit = hit_cylinder;
		(*index)++;
	}
}

void convert_objects(t_scene *scene)
{
	int index = 0;
	int total_objects = scene->sphere_count + scene->plane_count + scene->cylinder_count;

	scene->objects = malloc(sizeof(t_object) * total_objects);
	if (!scene->objects)
		exit_error("Memory allocation failed for unified objects");
	scene->object_count = 0;

	convert_spheres(scene, &index);
	convert_planes(scene, &index);
	convert_cylinders(scene, &index);

    // Verify the total count
	scene->object_count = index;
}
