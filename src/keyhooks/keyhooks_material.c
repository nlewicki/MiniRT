/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyhooks_material.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:36:57 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/19 13:07:12 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static bool	adjust_object_reflection(t_object *obj, double delta)
{
	if (obj->type == SPHERE)
		return (handle_sphere_reflection((t_sphere *)obj->data, delta));
	if (obj->type == CYLINDER)
		return (handle_cylinder_reflection((t_cylinder *)obj->data, delta));
	if (obj->type == CONE)
		return (handle_cone_reflection((t_cone *)obj->data, delta));
	return (false);
}

static bool	adjust_reflection(t_scene *scene, double delta)
{
	bool		changed;
	int			i;

	changed = false;
	i = -1;
	while (++i < scene->object_count)
		if (adjust_object_reflection(&scene->objects[i], delta))
			changed = true;
	return (changed);
}

bool	handle_material_property(t_miniRT *mini, int key)
{
	const double	reflection_step = 0.1;

	if (key == MLX_KEY_1)
		return (adjust_reflection(&mini->scene, reflection_step));
	else if (key == MLX_KEY_2)
		return (adjust_reflection(&mini->scene, -reflection_step));
	return (false);
}
