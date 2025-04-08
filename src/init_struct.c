/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:46:10 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/08 11:41:34 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	init_scene(t_scene *scene)
{
	scene->ambient.is_set = 0;
	scene->camera.is_set = 0;
	scene->lights = NULL;
    scene->light_count = 0;
	scene->spheres = NULL;
	scene->sphere_count = 0;
	scene->planes = NULL;
	scene->plane_count = 0;
	scene->cylinders = NULL;
	scene->cylinder_count = 0;
	printf("Scene initialized: lights=%d, spheres=%d\n", scene->light_count, scene->sphere_count);
}
