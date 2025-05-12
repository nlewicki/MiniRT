/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:47:00 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/12 13:03:01 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	free_sphere_data(void *data)
{
	t_sphere *sphere;

	if (!data)
		return;
	sphere = (t_sphere *)data;
	cleanup_texture(&sphere->texture);
	free(data);
}

static void	free_plane_data(void *data)
{
	t_plane *plane;

	if (!data)
		return;
	plane = (t_plane *)data;
	cleanup_texture(&plane->texture);
	free(data);
}

static void	free_cylinder_data(void *data)
{
	t_cylinder *cylinder;

	if (!data)
		return;
	cylinder = (t_cylinder *)data;
	cleanup_texture(&cylinder->texture);
	free(data);
}

static void	free_cone_data(void *data)
{
	t_cone *cone;

	if (!data)
		return;
	cone = (t_cone *)data;
	cleanup_texture(&cone->texture);
	free(data);
}

void	free_object(t_object *obj)
{
	if (!obj)
		return;

	switch (obj->type)
	{
		case SPHERE:
			free_sphere_data(obj->data);
			break;
		case PLANE:
			free_plane_data(obj->data);
			break;
		case CYLINDER:
			free_cylinder_data(obj->data);
			break;
		case CONE:
			free_cone_data(obj->data);
			break;
		default:
			if (obj->data)
				free(obj->data);
	}
}

