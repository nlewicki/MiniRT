/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:57:00 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/27 14:54:22 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	parse_param(char **tokens, t_cone *cone, double *angle_degrees)
{
	int	error;

	error = 0;
	cone->apex = parse_position(tokens[1], &error);
	if (error)
		exit_error("Invalid cone apex position");
	cone->direction = parse_orientation(tokens[2], &error);
	if (error)
		exit_error("Invalid cone direction");
	*angle_degrees = parse_double(tokens[3], 0.0, 180.0, &error);
	if (error)
		exit_error("Invalid cone angle");
	cone->angle = *angle_degrees * M_PI / 180.0;
	cone->height = parse_double(tokens[4], 0.0, INFINITY, &error);
	if (error)
		exit_error("Invalid cone height");
	cone->color = parse_color(tokens[5], &error);
	if (error)
		exit_error("Invalid cone color");
	cone->checker = false;
}

static void	add_cone_to_scene(t_scene *scene, t_cone cone)
{
	t_cone	*new_cones;

	new_cones = ft_realloc(scene->cones,
			sizeof(t_cone) * scene->cone_count,
			sizeof(t_cone) * (scene->cone_count + 1));
	if (!new_cones)
		exit_error("Memory allocation failed for cones");
	scene->cones = new_cones;
	scene->cones[scene->cone_count] = cone;
	scene->cone_count++;
	scene->cones->reflection = REFLECTION;
}

void	parse_cone(char **tokens, t_scene *scene)
{
	t_cone	cone;
	double	angle_degrees;

	if (!tokens[1] || !tokens[2] || !tokens[3]
		|| !tokens[4] || !tokens[5] || tokens[6])
		exit_error("Invalid cone format");
	parse_param(tokens, &cone, &angle_degrees);
	add_cone_to_scene(scene, cone);
	printf("Cone added: apex=(%.2f, %.2f, %.2f), direction=(%.2f, %.2f, %.2f), "
		"angle=%.2f, height=%.2f, color=(%d, %d, %d)\n",
		cone.apex.x, cone.apex.y, cone.apex.z,
		cone.direction.x, cone.direction.y, cone.direction.z,
		angle_degrees, cone.height,
		cone.color.r, cone.color.g, cone.color.b);
	printf("\n");
}
