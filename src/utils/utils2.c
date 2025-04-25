/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 13:14:05 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/25 11:23:10 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_vec3	vec_cross(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y
		- a.y * b.x});
}

double	vec_length(t_vec3 v)
{
	return (sqrt(vec_dot(v, v)));
}

t_vec3	vec_normalize(t_vec3 v)
{
	double	len;

	len = vec_length(v);
	if (len == 0.0)
		return ((t_vec3){0, 0, 0});
	return (vec_mul(v, 1.0 / len));
}

t_vec3 vec_reflect(t_vec3 v, t_vec3 n)
{
	return vec_sub(vec_mul(n, 2 * vec_skal(v, n)), v);
}