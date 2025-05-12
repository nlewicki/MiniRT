/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:23:34 by nicolewicki       #+#    #+#             */
/*   Updated: 2025/05/12 13:19:32 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	parse_ambient(char **tokens, t_scene *scene)
{
	int	error;

	error = 0;
	if (scene->ambient.is_set)
		exit_error("Ambient light already defined");
	if (!tokens[1] || !tokens[2] || tokens[3])
		exit_error("Invalid ambient format");
	scene->ambient.ratio = parse_double(tokens[1], 0.0, 1.0, &error);
	scene->ambient.color = parse_color(tokens[2], &error);
	if (error)
		exit_error("Invalid ambient values");
	scene->ambient.is_set = 1;
	printf("Ambient light set: ratio=%.2f, color=(%d, %d, %d)\n",
		scene->ambient.ratio, scene->ambient.color.r,
		scene->ambient.color.g, scene->ambient.color.b);
	write(1, "\n", 2);
}
