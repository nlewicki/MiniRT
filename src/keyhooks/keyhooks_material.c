/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyhooks_material.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:36:57 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/16 11:53:58 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static bool	handle_sphere_reflection(t_sphere *sphere, double delta)
{
	double	new_value;

	new_value = fmax(REFLECTION_MIN, fmin(REFLECTION_MAX, sphere->reflection + delta));
	if (new_value != sphere->reflection)
	{
		sphere->reflection = new_value;
		printf("OBJECT TYPE: SPHERE\nREFLECTION: %f\n", sphere->reflection);
		return (true);
	}
	return (false);
}

static bool	handle_cylinder_reflection(t_cylinder *cylinder, double delta)
{
	double	new_value;

	new_value = fmax(REFLECTION_MIN, fmin(REFLECTION_MAX, cylinder->reflection + delta));
	if (new_value != cylinder->reflection)
	{
		cylinder->reflection = new_value;
		printf("OBJECT TYPE: CYLINDER\nREFLECTION: %f\n", cylinder->reflection);
		return (true);
	}
	return (false);
}

static bool	handle_cone_reflection(t_cone *cone, double delta)
{
	double	new_value;

	new_value = fmax(REFLECTION_MIN, fmin(REFLECTION_MAX, cone->reflection + delta));
	if (new_value != cone->reflection)
	{
		cone->reflection = new_value;
		printf("OBJECT TYPE: CONE\nREFLECTION: %f\n", cone->reflection);
		return (true);
	}
	return (false);
}

static bool	adjust_reflection(t_scene *scene, double delta)
{
	bool		changed;
	int			i;
	t_object	*obj;

	changed = false;
	i = 0;
	while (i < scene->object_count)
	{
		obj = &scene->objects[i];
		if (obj->type == SPHERE)
		{
			if (handle_sphere_reflection((t_sphere *)obj->data, delta))
				changed = true;
		}
		else if (obj->type == CYLINDER)
		{
			if (handle_cylinder_reflection((t_cylinder *)obj->data, delta))
				changed = true;
		}
		else if (obj->type == CONE)
		{
			if (handle_cone_reflection((t_cone *)obj->data, delta))
				changed = true;
		}
		i++;
	}
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
