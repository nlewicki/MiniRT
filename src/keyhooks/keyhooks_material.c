/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyhooks_material.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:36:57 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/13 12:02:49 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static bool	handle_sphere_property(t_sphere *sphere, int property_type, double delta)
{
	double	new_value;

	if (property_type == 0)
	{
		new_value = fmax(KS_MIN, fmin(KS_MAX, sphere->ks + delta));
		if (new_value != sphere->ks)
		{
			sphere->ks = new_value;
			printf("OBJECT TYPE: SPHERE\nKS: %f\n", sphere->ks);
			return (true);
		}
	}
	else if (property_type == 1)
	{
		new_value = fmax(SHINE_MIN, fmin(SHINE_MAX, sphere->shine + delta));
		if (new_value != sphere->shine)
		{
			sphere->shine = new_value;
			printf("OBJECT TYPE: SPHERE\nSHINE: %f\n", sphere->shine);
			return (true);
		}
	}
	else if (property_type == 2)
	{
		new_value = fmax(REFLECTION_MIN, fmin(REFLECTION_MAX, sphere->reflection + delta));
		if (new_value != sphere->reflection)
		{
			sphere->reflection = new_value;
			printf("OBJECT TYPE: SPHERE\nREFLECTION: %f\n", sphere->reflection);
			return (true);
		}
	}
	return (false);
}

static bool	handle_cylinder_property(t_cylinder *cylinder, int property_type, double delta)
{
	double	new_value;

	if (property_type == 0)
	{
		new_value = fmax(KS_MIN, fmin(KS_MAX, cylinder->ks + delta));
		if (new_value != cylinder->ks)
		{
			cylinder->ks = new_value;
			printf("OBJECT TYPE: CYLINDER\nKS: %f\n", cylinder->ks);
			return (true);
		}
	}
	else if (property_type == 1)
	{
		new_value = fmax(SHINE_MIN, fmin(SHINE_MAX, cylinder->shine + delta));
		if (new_value != cylinder->shine)
		{
			cylinder->shine = new_value;
			printf("OBJECT TYPE: CYLINDER\nSHINE: %f\n", cylinder->shine);
			return (true);
		}
	}
	else if (property_type == 2)
	{
		new_value = fmax(REFLECTION_MIN, fmin(REFLECTION_MAX, cylinder->reflection + delta));
		if (new_value != cylinder->reflection)
		{
			cylinder->reflection = new_value;
			printf("OBJECT TYPE: CYLINDER\nREFLECTION: %f\n", cylinder->reflection);
			return (true);
		}
	}
	return (false);
}

static bool	handle_cone_property(t_cone *cone, int property_type, double delta)
{
	double	new_value;

	if (property_type == 0)
	{
		new_value = fmax(KS_MIN, fmin(KS_MAX, cone->ks + delta));
		if (new_value != cone->ks)
		{
			cone->ks = new_value;
			printf("OBJECT TYPE: CONE\nKS: %f\n", cone->ks);
			return (true);
		}
	}
	else if (property_type == 1)
	{
		new_value = fmax(SHINE_MIN, fmin(SHINE_MAX, cone->shine + delta));
		if (new_value != cone->shine)
		{
			cone->shine = new_value;
			printf("OBJECT TYPE: CONE\nSHINE: %f\n", cone->shine);
			return (true);
		}
	}
	else if (property_type == 2)
	{
		new_value = fmax(REFLECTION_MIN, fmin(REFLECTION_MAX, cone->reflection + delta));
		if (new_value != cone->reflection)
		{
			cone->reflection = new_value;
			printf("OBJECT TYPE: CONE\nREFLECTION: %f\n", cone->reflection);
			return (true);
		}
	}
	return (false);
}

static bool	adjust_material_property(t_scene *scene, int property_type, double delta)
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
			if (handle_sphere_property((t_sphere *)obj->data, property_type, delta))
				changed = true;
		}
		else if (obj->type == CYLINDER)
		{
			if (handle_cylinder_property((t_cylinder *)obj->data, property_type, delta))
				changed = true;
		}
		else if (obj->type == CONE)
		{
			if (handle_cone_property((t_cone *)obj->data, property_type, delta))
				changed = true;
		}
		i++;
	}
	return (changed);
}

bool	handle_material_property(t_miniRT *mini, int key)
{
	const double	ks_step = 0.1;
	const double	shine_step = 5.0;
	const double	reflection_step = 0.1;

	if (key == MLX_KEY_1)
		return (adjust_material_property(&mini->scene, 0, ks_step));
	else if (key == MLX_KEY_2)
		return (adjust_material_property(&mini->scene, 0, -ks_step));
	else if (key == MLX_KEY_3)
		return (adjust_material_property(&mini->scene, 1, shine_step));
	else if (key == MLX_KEY_4)
		return (adjust_material_property(&mini->scene, 1, -shine_step));
	else if (key == MLX_KEY_5)
		return (adjust_material_property(&mini->scene, 2, reflection_step));
	else if (key == MLX_KEY_6)
		return (adjust_material_property(&mini->scene, 2, -reflection_step));
	return (false);
}
