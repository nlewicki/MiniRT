/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leokubler <leokubler@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 12:50:00 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/26 10:17:11 by leokubler        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
// vlt überflüssig
double	solve_quadratic(double a, double b, double c)
{
	double	discriminant;
	double	q;
	double	t1;
	double	t2;
	double	tmp;

	if (fabs(a) < 1e-6)
		return (-1.0);
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (-1.0);
	if (b > 0)
		q = -0.5 * (b + sqrt(discriminant));
		
	else
		q = -0.5 * (b - sqrt(discriminant));
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

// Check and update cylinder caps
static void	check_cylinder_caps(t_cylinder_context *ctx)
{
	t_vec3	bottom_cap;
	t_vec3	top_cap;
	double	t_cap;

	bottom_cap = ctx->cyl->position;
	top_cap = vec_add(ctx->cyl->position, 
			vec_mul(ctx->cyl->orientation, ctx->cyl->height));
	t_cap = hit_cylinder_caps(ctx->obj, ctx->cyl, ctx->ray, NULL, bottom_cap);
	if (t_cap > 0 && t_cap < ctx->closest_t)
	{
		ctx->closest_t = t_cap;
		if (ctx->hit_info)
			hit_cylinder_caps(ctx->obj, ctx->cyl, ctx->ray, 
				ctx->hit_info, bottom_cap);
	}
	t_cap = hit_cylinder_caps(ctx->obj, ctx->cyl, ctx->ray, NULL, top_cap);
	if (t_cap > 0 && t_cap < ctx->closest_t)
	{
		ctx->closest_t = t_cap;
		if (ctx->hit_info)
			hit_cylinder_caps(ctx->obj, ctx->cyl, ctx->ray, 
				ctx->hit_info, top_cap);
	}
}

// Calculate cylinder body intersection
static double	calculate_body_intersection(t_cylinder_context *ctx)
{
	t_cylinder_math	math;
	double			a;
	double			b;
	double			c;

	math.oc = vec_sub(ctx->ray.origin, ctx->cyl->position);
	math.axis = ctx->cyl->orientation;
	math.dot_d_axis = vec_dot(ctx->ray.direction, math.axis);
	math.dot_oc_axis = vec_dot(math.oc, math.axis);
	a = vec_dot(ctx->ray.direction, ctx->ray.direction) 
		- math.dot_d_axis * math.dot_d_axis;
	b = 2 * (vec_dot(ctx->ray.direction, math.oc) 
		- math.dot_d_axis * math.dot_oc_axis);
	c = vec_dot(math.oc, math.oc) - math.dot_oc_axis * math.dot_oc_axis 
		- (ctx->cyl->diameter * ctx->cyl->diameter) / 4;
	return (solve_quadratic(a, b, c));
}

// Validate and set body hit information
static bool	process_body_hit(t_cylinder_context *ctx, double t_body)
{
	t_vec3	p;
	double	height;

	p = vec_add(ctx->ray.origin, vec_mul(ctx->ray.direction, t_body));
	height = vec_dot(vec_sub(p, ctx->cyl->position), ctx->cyl->orientation);
	if (height < -1e-6 || height > ctx->cyl->height + 1e-6)
		return (false);
	ctx->closest_t = t_body;
	if (ctx->hit_info)
	{
		ctx->hit_info->t = t_body;
		ctx->hit_info->point = p;
		ctx->hit_info->normal = vec_normalize(vec_sub(p, 
				vec_add(ctx->cyl->position, 
					vec_mul(ctx->cyl->orientation, height))));
		if (ctx->cyl->checker)
			ctx->hit_info->color = checkerboard_cylinder(ctx->cyl, p);
		else
			ctx->hit_info->color = ctx->obj->color;
		ctx->hit_info->object = ctx->obj;
	}
	return (true);
}

// Check cylinder body intersection
static void	check_cylinder_body(t_cylinder_context *ctx)
{
	double	t_body;

	t_body = calculate_body_intersection(ctx);
	if (t_body > 0 && t_body < ctx->closest_t)
		process_body_hit(ctx, t_body);
}

// Main cylinder intersection function
double	hit_cylinder(t_object *obj, const t_ray ray, t_hit *hit_info)
{
	t_cylinder_context	ctx;

	ctx.cyl = (t_cylinder *)obj->data;
	if (!ctx.cyl)
		return (-1.0);
	ctx.obj = obj;
	ctx.ray = ray;
	ctx.hit_info = hit_info;
	ctx.closest_t = INFINITY;
	ctx.cyl->orientation = vec_normalize(ctx.cyl->orientation);
	check_cylinder_caps(&ctx);
	check_cylinder_body(&ctx);
	if (ctx.closest_t == INFINITY)
		return (-1.0);
	return (ctx.closest_t);
}
