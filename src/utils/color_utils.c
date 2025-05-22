/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:47:50 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/22 12:54:06 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

t_color	color_scale(t_color c, double factor)
{
	t_color	result;

	result.r = c.r * factor;
	result.g = c.g * factor;
	result.b = c.b * factor;
	result.a = c.a;
	return (result);
}

t_color	color_add(t_color c1, t_color c2)
{
	t_color	result;

	result.r = c1.r + c2.r;
	result.g = c1.g + c2.g;
	result.b = c1.b + c2.b;
	result.a = c1.a;
	return (result);
}

t_color	color_mix(t_color a, t_color b, double factor)
{
	t_color	result;

	result.r = a.r * (1 - factor) + b.r * factor;
	result.g = a.g * (1 - factor) + b.g * factor;
	result.b = a.b * (1 - factor) + b.b * factor;
	return (result);
}


//static t_color color_mult(t_color c1, t_color c2)
//{
//	t_color result = {
//		.r = (c1.r * c2.r) / 255,
//		.g = (c1.g * c2.g) / 255,
//		.b = (c1.b * c2.b) / 255,
//		.a = c1.a
//	};
//	return result;
//}