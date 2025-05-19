/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyhooks_checker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:48:06 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/19 13:01:57 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static bool	toggle_sphere_checker(t_scene *scene)
{
	int			i;
	t_object	*obj;

	i = 0;
	while (i < scene->object_count)
	{
		obj = &scene->objects[i];
		if (obj->type == SPHERE)
			((t_sphere *)obj->data)->checker
				=!((t_sphere *)obj->data)->checker;
		i++;
	}
	return (true);
}

static bool	toggle_plane_checker(t_scene *scene)
{
	int			i;
	t_object	*obj;

	i = 0;
	while (i < scene->object_count)
	{
		obj = &scene->objects[i];
		if (obj->type == PLANE)
			((t_plane *)obj->data)->checker
				=!((t_plane *)obj->data)->checker;
		i++;
	}
	return (true);
}

static bool	toggle_cylinder_checker(t_scene *scene)
{
	int			i;
	t_object	*obj;

	i = 0;
	while (i < scene->object_count)
	{
		obj = &scene->objects[i];
		if (obj->type == CYLINDER)
			((t_cylinder *)obj->data)->checker
				=!((t_cylinder *)obj->data)->checker;
		i++;
	}
	return (true);
}

static bool	toggle_cone_checker(t_scene *scene)
{
	int			i;
	t_object	*obj;

	i = 0;
	while (i < scene->object_count)
	{
		obj = &scene->objects[i];
		if (obj->type == CONE)
			((t_cone *)obj->data)->checker
				=!((t_cone *)obj->data)->checker;
		i++;
	}
	return (true);
}

bool	handle_checkerboard_toggle(t_miniRT *mini, int key)
{
	if (key == MLX_KEY_Z)
		return (toggle_sphere_checker(&mini->scene));
	else if (key == MLX_KEY_X)
		return (toggle_plane_checker(&mini->scene));
	else if (key == MLX_KEY_C)
		return (toggle_cylinder_checker(&mini->scene));
	else if (key == MLX_KEY_V)
		return (toggle_cone_checker(&mini->scene));
	else if (key == MLX_KEY_B)
	{
		toggle_sphere_checker(&mini->scene);
		toggle_plane_checker(&mini->scene);
		toggle_cylinder_checker(&mini->scene);
		toggle_cone_checker(&mini->scene);
		return (true);
	}
	return (false);
}
