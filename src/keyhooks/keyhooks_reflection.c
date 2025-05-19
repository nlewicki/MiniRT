/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyhooks_reflection.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:06:56 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/19 13:07:29 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

bool	handle_sphere_reflection(t_sphere *sphere, double delta)
{
	double	new_value;

	new_value = fmax(REFLECTION_MIN,
			fmin(REFLECTION_MAX, sphere->reflection + delta));
	if (new_value != sphere->reflection)
	{
		sphere->reflection = new_value;
		printf("OBJECT TYPE: SPHERE\nREFLECTION: %f\n", sphere->reflection);
		return (true);
	}
	return (false);
}

bool	handle_cylinder_reflection(t_cylinder *cylinder, double delta)
{
	double	new_value;

	new_value = fmax(REFLECTION_MIN,
			fmin(REFLECTION_MAX, cylinder->reflection + delta));
	if (new_value != cylinder->reflection)
	{
		cylinder->reflection = new_value;
		printf("OBJECT TYPE: CYLINDER\nREFLECTION: %f\n", cylinder->reflection);
		return (true);
	}
	return (false);
}

bool	handle_cone_reflection(t_cone *cone, double delta)
{
	double	new_value;

	new_value = fmax(REFLECTION_MIN,
			fmin(REFLECTION_MAX, cone->reflection + delta));
	if (new_value != cone->reflection)
	{
		cone->reflection = new_value;
		printf("OBJECT TYPE: CONE\nREFLECTION: %f\n", cone->reflection);
		return (true);
	}
	return (false);
}
