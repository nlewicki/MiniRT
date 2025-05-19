/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:57:37 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/19 14:15:31 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

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
