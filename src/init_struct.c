/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:46:10 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/07 13:21:45 by nlewicki         ###   ########.fr       */
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
	scene->planes = NULL;
	scene->cylinders = NULL;
}
