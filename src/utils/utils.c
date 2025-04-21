/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:08:57 by lkubler           #+#    #+#             */
/*   Updated: 2025/04/21 14:53:59 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_vec3 vec_sub(t_vec3 a, t_vec3 b)
{
	return (t_vec3){a.x - b.x, a.y - b.y, a.z - b.z};
}

double vec_skal(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

t_vec3 vec_add(t_vec3 a, t_vec3 b)
{
	return (t_vec3){a.x + b.x, a.y + b.y, a.z + b.z};
}


t_vec3 vec_mul(t_vec3 v, double scalar)
{
	return (t_vec3){v.x * scalar, v.y * scalar, v.z * scalar};
}

double vec_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3 vec_cross(t_vec3 a, t_vec3 b)
{
	return (t_vec3){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

double vec_length(t_vec3 v)
{
	return sqrt(vec_dot(v, v));
}

t_vec3 vec_normalize(t_vec3 v)
{
	double len = vec_length(v);
	if (len == 0.0)
		return (t_vec3){0, 0, 0}; // Sicherheits-Check
	return vec_mul(v, 1.0 / len);
}
