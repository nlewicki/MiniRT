/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 11:46:10 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/30 12:11:10 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void init_material_properties(t_miniRT *mini)
{
    mini->default_material.ks = 0.5;         // Default specular coefficient
    mini->default_material.shine = 50.0;     // Default shininess
    mini->default_material.reflection = 0.4; // Default reflection
}

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
	scene->objects = NULL;
	scene->object_count = 0;
}

void init_miniRT(t_miniRT *mini)
{
	init_scene(&mini->scene);
	mini->low_res_mode = true;
	mini->res_scale = 4;
	mini->samples = 2;
	init_material_properties(mini);
}
