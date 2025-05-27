/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_ray.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:24:54 by lkubler           #+#    #+#             */
/*   Updated: 2025/05/27 15:04:01 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Find closest hit in scene
t_hit	find_closest_hit(t_miniRT *mini, t_ray ray, t_object **hit_object)
{
	t_hit	closest_hit;
	t_hit	temp_hit;
	double	closest;
	double	t;
	int		i;

	closest = 1e30;
	i = 0;
	*hit_object = NULL;
	while (i < mini->scene.object_count)
	{
		t = mini->scene.objects[i].hit(&mini->scene.objects[i], ray, &temp_hit);
		if (t > 0 && t < closest)
		{
			closest = t;
			closest_hit = temp_hit;
			*hit_object = &mini->scene.objects[i];
		}
		i++;
	}
	return (closest_hit);
}

// Find closest hit in scene, skipping a specific object
t_hit	find_closest_hit_skip(t_miniRT *mini, t_ray ray, t_object *skip_object)
{
	t_hit	closest_hit;
	t_hit	temp_hit;
	double	closest;
	double	t;
	int		i;

	closest = 1e30;
	i = 0;
	closest_hit.object = NULL;
	while (i < mini->scene.object_count)
	{
		if (&mini->scene.objects[i] != skip_object)
		{
			t = mini->scene.objects[i].hit(&mini->scene.objects[i],
					ray, &temp_hit);
			if (t > 0 && t < closest)
			{
				closest = t;
				closest_hit = temp_hit;
			}
		}
		i++;
	}
	return (closest_hit);
}

// Main ray tracing function
t_color	trace_ray(t_miniRT *mini, t_ray ray, int depth)
{
	t_hit		closest_hit;
	t_object	*hit_object;
	bool		hit_any;

	if (depth <= 0)
		return (color_scale(mini->scene.ambient.color,
				mini->scene.ambient.ratio));
	closest_hit = find_closest_hit(mini, ray, &hit_object);
	hit_any = (hit_object != NULL);
	if (hit_any)
		return (handle_reflection(mini, closest_hit, ray, depth));
	if (!mini->scene.ambient.is_set || mini->scene.ambient.ratio <= 0.0)
		return ((t_color){30, 30, 30, 255});
	return (color_scale(mini->scene.ambient.color, mini->scene.ambient.ratio));
}

// Trace a ray while skipping a specific object
t_color	trace_ray_skip_object(t_miniRT *mini, t_ray ray, int depth,
							t_object *skip_object)
{
	t_hit					closest_hit;
	bool					hit_any;
	t_reflection_context	context;

	if (depth <= 0)
		return (color_scale(mini->scene.ambient.color,
				mini->scene.ambient.ratio));
	closest_hit = find_closest_hit_skip(mini, ray, skip_object);
	hit_any = (closest_hit.object != NULL);
	if (hit_any)
	{
		context.ray = ray;
		context.depth = depth;
		context.skip_object = skip_object;
		return (handle_reflection_skip(mini, closest_hit, context));
	}
	if (!mini->scene.ambient.is_set || mini->scene.ambient.ratio <= 0.0)
		return ((t_color){30, 30, 30, 255});
	return (color_scale(mini->scene.ambient.color, mini->scene.ambient.ratio));
}

//double	calculate_light_factors(t_light_context ctx, t_light light)
//{
//	t_vec3	light_dir;
//	double	shadow;
//	double	diffuse;
//	double	dist;
//	double	falloff;

//	light_dir = vec_normalize(vec_sub(light.position, ctx.hit.point));
//	shadow = compute_shadow_factor(ctx.mini, ctx.hit.point, light,
//			ctx.skip_object);
//	shadow = pow(shadow, 2);
//	if (shadow <= 0.0)
//		return (-1.0);
//	dist = vec_length(vec_sub(light.position, ctx.hit.point));
//	falloff = 2.0 / (1.0 + 0.1 * dist * dist);
//	diffuse = fmax(0.0, vec_skal(ctx.hit.normal, light_dir));
//	return (diffuse * light.brightness * shadow * falloff);
//}
