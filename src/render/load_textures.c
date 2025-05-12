/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:15:00 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/12 13:01:10 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static bool	load_sphere_textures(t_scene *scene, mlx_t *mlx)
{
	int	i;

	i = 0;
	while (i < scene->sphere_count)
	{
		if (scene->spheres[i].texture.path)
		{
			if (!load_texture(&scene->spheres[i].texture,
					scene->spheres[i].texture.path, mlx))
			{
				printf("Warning: Failed to load texture for sphere %d: %s\n",
					i, scene->spheres[i].texture.path);
				cleanup_texture(&scene->spheres[i].texture);
			}
		}
		i++;
	}
	return (true);
}

static bool	load_plane_textures(t_scene *scene, mlx_t *mlx)
{
	int	i;

	i = 0;
	while (i < scene->plane_count)
	{
		if (scene->planes[i].texture.path)
		{
			if (!load_texture(&scene->planes[i].texture,
					scene->planes[i].texture.path, mlx))
			{
				printf("Warning: Failed to load texture for plane %d: %s\n",
					i, scene->planes[i].texture.path);
				cleanup_texture(&scene->planes[i].texture);
			}
		}
		i++;
	}
	return (true);
}

static bool	load_cylinder_textures(t_scene *scene, mlx_t *mlx)
{
	int	i;

	i = 0;
	while (i < scene->cylinder_count)
	{
		if (scene->cylinders[i].texture.path)
		{
			if (!load_texture(&scene->cylinders[i].texture,
					scene->cylinders[i].texture.path, mlx))
			{
				printf("Warning: Failed to load texture for cylinder %d: %s\n",
					i, scene->cylinders[i].texture.path);
				cleanup_texture(&scene->cylinders[i].texture);
			}
		}
		i++;
	}
	return (true);
}

static bool	load_cone_textures(t_scene *scene, mlx_t *mlx)
{
	int	i;

	i = 0;
	while (i < scene->cone_count)
	{
		if (scene->cones[i].texture.path)
		{
			if (!load_texture(&scene->cones[i].texture,
					scene->cones[i].texture.path, mlx))
			{
				printf("Warning: Failed to load texture for cone %d: %s\n",
					i, scene->cones[i].texture.path);
				cleanup_texture(&scene->cones[i].texture);
			}
		}
		i++;
	}
	return (true);
}

bool	load_scene_textures(t_scene *scene, mlx_t *mlx)
{
	if (!scene || !mlx)
		return (false);
	if (!load_sphere_textures(scene, mlx))
		return (false);
	if (!load_plane_textures(scene, mlx))
		return (false);
	if (!load_cylinder_textures(scene, mlx))
		return (false);
	if (!load_cone_textures(scene, mlx))
		return (false);
	return (true);
}

void	cleanup_scene_textures(t_scene *scene)
{
	int	i;

	if (!scene)
		return ;
	i = 0;
	while (i < scene->sphere_count)
		cleanup_texture(&scene->spheres[i++].texture);
	i = 0;
	while (i < scene->plane_count)
		cleanup_texture(&scene->planes[i++].texture);
	i = 0;
	while (i < scene->cylinder_count)
		cleanup_texture(&scene->cylinders[i++].texture);
	i = 0;
	while (i < scene->cone_count)
		cleanup_texture(&scene->cones[i++].texture);
}
