/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sphere.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolewicki <nicolewicki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:24:31 by nicolewicki       #+#    #+#             */
/*   Updated: 2025/04/09 15:28:39 by nicolewicki      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void parse_sphere(char **tokens, t_scene *scene)
{
    int error = 0;
    t_sphere *new_sphere;

    if (!tokens[1] || !tokens[2] || !tokens[3] || tokens[4]) // Expect exactly 4 tokens: "sp", center, diameter, color
        exit_error("Invalid sphere format");

    t_sphere sphere;
    sphere.center = parse_position(tokens[1], &error);
    if (error)
        exit_error("Invalid sphere center");
    sphere.diameter = parse_double(tokens[2], 0.0, INFINITY, &error);
    if (error)
        exit_error("Invalid sphere diameter");
    sphere.color = parse_color(tokens[3], &error);
    if (error)
        exit_error("Invalid sphere color");

    new_sphere = ft_realloc(scene->spheres, sizeof(t_sphere) * (scene->sphere_count + 1));
    if (!new_sphere)
        exit_error("Memory allocation failed for spheres");
    scene->spheres = new_sphere;
    scene->spheres[scene->sphere_count] = sphere;
    scene->sphere_count++;

    // Debug print
    printf("Sphere added: center=(%.2f, %.2f, %.2f), diameter=%.2f, color=(%d, %d, %d)\n",
           sphere.center.x, sphere.center.y, sphere.center.z, sphere.diameter,
           sphere.color.r, sphere.color.g, sphere.color.b);
    printf("\n"); // Simpler than write
}