/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolewicki <nicolewicki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:57:37 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/26 12:41:47 by nicolewicki      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static double	calculate_discriminant(double a, double b, double c, double *q)
{
	double	discriminant;

	if (fabs(a) < 1e-6)
		return (-1.0);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	if (b > 0)
		*q = -0.5 * (b + sqrt(discriminant));
	else
		*q = -0.5 * (b - sqrt(discriminant));
	return (discriminant);
}

double	solve_quadratic(double a, double b, double c)
{
	double	discriminant;
	double	q;
	double	t1;
	double	t2;
	double	tmp;

	discriminant = calculate_discriminant(a, b, c, &q);
	if (discriminant < 0)
		return (-1.0);
	t1 = q / a;
	t2 = c / q;
	if (t1 > t2)
	{
		tmp = t1;
		t1 = t2;
		t2 = tmp;
	}
	if (t1 > 0)
		return (t1);
	if (t2 > 0)
		return (t2);
	return (-1.0);
}

t_vec3	get_local_coords(t_vec3 point, t_vec3 origin)
{
	return (vec_normalize(vec_sub(point, origin)));
}

void	set_base_hit_normal(t_hit *hit_info, t_vec3 normal, double denom)
{
	if (denom > 0)
		hit_info->normal = vec_mul(normal, -1);
	else
		hit_info->normal = normal;
}

void	check_mode(t_hit *hit_info, t_cone *cone, t_object *obj)
{
	if (cone->checker)
		hit_info->color = checkerboard_cone(cone, hit_info->point);
	else
		hit_info->color = obj->color;
}
