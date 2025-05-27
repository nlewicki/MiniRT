/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:37:42 by nicolewicki       #+#    #+#             */
/*   Updated: 2025/05/27 15:29:56 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	set_cap_hit_info(t_cylinder_context *ctx, t_cap_hit *hit)
{
	ctx->hit_info->t = hit->t;
	ctx->hit_info->point = hit->point;
	if (hit->denom > 0)
		ctx->hit_info->normal = hit->normal;
	else
		ctx->hit_info->normal = vec_mul(hit->normal, -1);
	ctx->hit_info->object = ctx->obj;
	if (ctx->cyl->checker)
		ctx->hit_info->color = checkerboard_cylinder(ctx->cyl, hit->point);
	else
		ctx->hit_info->color = ctx->obj->color;
}

double	hit_cylinder_caps(t_cylinder_context *ctx, t_vec3 cap_center)
{
	t_cap_hit	hit;

	hit.normal = ctx->cyl->orientation;
	hit.denom = vec_dot(ctx->ray.direction, hit.normal);
	if (fabs(hit.denom) < 1e-6)
		return (-1.0);
	hit.t = vec_dot(vec_sub(cap_center, ctx->ray.origin),
			hit.normal) / hit.denom;
	if (hit.t < 0)
		return (-1.0);
	hit.point = vec_add(ctx->ray.origin,
			vec_mul(ctx->ray.direction, hit.t));
	if (vec_length(vec_sub(hit.point, cap_center))
		> (ctx->cyl->diameter / 2) + 1e-6)
		return (-1.0);
	if (ctx->hit_info)
		set_cap_hit_info(ctx, &hit);
	return (hit.t);
}
