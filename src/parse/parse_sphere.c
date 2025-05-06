/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:24:31 by nicolewicki       #+#    #+#             */
/*   Updated: 2025/05/06 11:41:48 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	parse_sphere(char **tokens, t_scene *scene)
{
	int			error;
	t_sphere	*new_sphere;
	t_sphere	sphere;

	error = 0;
	if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4])
		exit_error("Invalid sphere format");
	sphere.center = parse_position(tokens[1], &error);
	if (error)
		exit_error("Invalid sphere center");
	sphere.diameter = parse_double(tokens[2], 0.0, INFINITY, &error);
	if (error)
		exit_error("Invalid sphere diameter");
	sphere.color = parse_color(tokens[3], &error);
	if (error)
		exit_error("Invalid sphere color");
	new_sphere = ft_realloc(scene->spheres,
			sizeof(t_sphere) * (scene->sphere_count + 1));
	if (!new_sphere)
		exit_error("Memory allocation failed for spheres");
	scene->spheres = new_sphere;
	scene->spheres[scene->sphere_count] = sphere;
	scene->sphere_count++;
	scene->spheres->reflection = REFLECTION;
	scene->spheres->shine = SHINE;
	scene->spheres->ks = KS;
	// Debug print
	printf("Sphere added: center=(%.2f, %.2f, %.2f), diameter=%.2f, color=(%d, %d, %d)\n",
		sphere.center.x, sphere.center.y, sphere.center.z, sphere.diameter,
		sphere.color.r, sphere.color.g, sphere.color.b);
	printf("\n"); // Simpler than write
}
