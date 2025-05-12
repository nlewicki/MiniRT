/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_object.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 14:23:41 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/12 13:29:27 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	convert_spheres(t_scene *scene, int *index)
{
	int			i;
	t_sphere	*sphere_data;

	i = 0;
	while (i < scene->sphere_count)
	{
		sphere_data = malloc(sizeof(t_sphere));
		if (!sphere_data)
			exit_error("Memory allocation failed for sphere data");
		*sphere_data = scene->spheres[i];
		scene->objects[*index].type = SPHERE;
		scene->objects[*index].data = sphere_data;
		scene->objects[*index].color = sphere_data->color;
		scene->objects[*index].material_link = sphere_data->material_link;
		scene->objects[*index].hit = hit_sphere;
		(*index)++;
		i++;
	}
}

void	convert_planes(t_scene *scene, int *index)
{
	int		i;
	t_plane	*plane_data;

	i = 0;
	while (i < scene->plane_count)
	{
		plane_data = malloc(sizeof(t_plane));
		if (!plane_data)
			exit_error("Memory allocation failed for plane data");
		*plane_data = scene->planes[i];
		scene->objects[*index].type = PLANE;
		scene->objects[*index].data = plane_data;
		scene->objects[*index].color = plane_data->color;
		scene->objects[*index].material_link = plane_data->material_link;
		scene->objects[*index].hit = hit_plane;
		(*index)++;
		i++;
	}
}

void	convert_cylinders(t_scene *scene, int *index)
{
	int			i;
	t_cylinder	*cylinder_data;

	i = 0;
	while (i < scene->cylinder_count)
	{
		cylinder_data = malloc(sizeof(t_cylinder));
		if (!cylinder_data)
			exit_error("Memory allocation failed for cylinder data");
		*cylinder_data = scene->cylinders[i];
		scene->objects[*index].type = CYLINDER;
		scene->objects[*index].data = cylinder_data;
		scene->objects[*index].color = cylinder_data->color;
		scene->objects[*index].material_link = cylinder_data->material_link;
		scene->objects[*index].hit = hit_cylinder;
		(*index)++;
		i++;
	}
}

void	convert_cones(t_scene *scene, int *index)
{
	int			i;
	t_cone		*cone_data;

	i = 0;
	while (i < scene->cone_count)
	{
		cone_data = malloc(sizeof(t_cone));
		if (!cone_data)
			exit_error("Memory allocation failed for cone data");
		*cone_data = scene->cones[i];
		scene->objects[*index].type = CONE;
		scene->objects[*index].data = cone_data;
		scene->objects[*index].color = cone_data->color;
		scene->objects[*index].material_link = cone_data->material_link;
		scene->objects[*index].hit = hit_cone;
		(*index)++;
		i++;
	}
}

void	convert_objects(t_scene *scene)
{
	int	index;
	int	total_objects;

	total_objects = scene->sphere_count
		+ scene->plane_count + scene->cylinder_count + scene->cone_count;
	index = 0;
	scene->objects = malloc(sizeof(t_object) * total_objects);
	if (!scene->objects)
		exit_error("Memory allocation failed for unified objects");
	scene->object_count = 0;
	convert_spheres(scene, &index);
	convert_planes(scene, &index);
	convert_cylinders(scene, &index);
	convert_cones(scene, &index);
	scene->object_count = index;
}
