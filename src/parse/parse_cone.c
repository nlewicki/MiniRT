/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cone.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 11:57:00 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/12 12:34:42 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	parse_cone(char **tokens, t_scene *scene)
{
	int			error;
	t_cone		*new_cones;
	t_cone		cone;
	double		angle_degrees;

	error = 0;
	if (!tokens[1] || !tokens[2] || !tokens[3]
		|| !tokens[4] || !tokens[5])
		exit_error("Invalid cone format");
	cone.apex = parse_position(tokens[1], &error);
	if (error)
		exit_error("Invalid cone apex position");
	cone.direction = parse_orientation(tokens[2], &error);
	if (error)
		exit_error("Invalid cone direction");
	angle_degrees = parse_double(tokens[3], 0.0, 180.0, &error);
	if (error)
		exit_error("Invalid cone angle");
	cone.angle = angle_degrees * M_PI / 180.0;  // Convert to radians
	cone.height = parse_double(tokens[4], 0.0, INFINITY, &error);
	if (error)
		exit_error("Invalid cone height");
	cone.color = parse_color(tokens[5], &error);
	if (error)
		exit_error("Invalid cone color");
	cone.checker = false;
	init_texture(&cone.texture);
	if (tokens[6] && tokens[6][0])
		cone.texture.path = ft_strdup(tokens[6]);
	new_cones = ft_realloc(scene->cones,
			sizeof(t_cone) * (scene->cone_count + 1));
	if (!new_cones)
		exit_error("Memory allocation failed for cones");
	scene->cones = new_cones;
	scene->cones[scene->cone_count] = cone;
	scene->cone_count++;
	scene->cones->reflection = REFLECTION;
	scene->cones->shine = SHINE;
	scene->cones->ks = KS;
	// Debug print
	printf("Cone added: apex=(%.2f, %.2f, %.2f), direction=(%.2f, %.2f, %.2f), angle=%.2f, height=%.2f, color=(%d, %d, %d), texture=%s\n",
		cone.apex.x, cone.apex.y, cone.apex.z,
		cone.direction.x, cone.direction.y, cone.direction.z,
		angle_degrees, cone.height,
		cone.color.r, cone.color.g, cone.color.b,
		cone.texture.path ? cone.texture.path : "none");
	printf("\n");
}
