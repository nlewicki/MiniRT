/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 12:01:17 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/27 13:43:29 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	parse_double(char *str, double min, double max, int *error)
{
	char	*endptr;
	double	value;

	errno = 0;
	value = strtod(str, &endptr);
	if (errno == ERANGE || *endptr != '\0' || value < min || value > max)
	{
		*error = 1;
		return (0);
	}
	return (value);
}

t_color	parse_color(char *str, int *error)
{
	char	**rgb;
	t_color	color;

	rgb = ft_split(str, ',');
	if (!rgb || !rgb[0] || !rgb[1] || !rgb[2])
	{
		if (rgb)
			ft_free_split(rgb);
		*error = 1;
		return ((t_color){0, 0, 0, 0});
	}
	color.r = (int)parse_double(rgb[0], 0, 255, error);
	color.g = (int)parse_double(rgb[1], 0, 255, error);
	color.b = (int)parse_double(rgb[2], 0, 255, error);
	color.a = 255;
	if (*error)
	{
		ft_free_split(rgb);
		*error = 1;
		return ((t_color){0, 0, 0, 0});
	}
	ft_free_split(rgb);
	return (color);
}

t_vec3	parse_position(char *str, int *error)
{
	char	**coords;
	t_vec3	vec;

	coords = ft_split(str, ',');
	if (!coords || !coords[0] || !coords[1] || !coords[2])
	{
		*error = 1;
		ft_free_split(coords);
		return ((t_vec3){0, 0, 0});
	}
	vec.x = parse_double(coords[0], -INFINITY, INFINITY, error);
	vec.y = parse_double(coords[1], -INFINITY, INFINITY, error);
	vec.z = parse_double(coords[2], -INFINITY, INFINITY, error);
	ft_free_split(coords);
	if (*error)
		return ((t_vec3){0, 0, 0});
	return (vec);
}

t_vec3	normalize_vector(t_vec3 vec, int *error)
{
	double	magnitude;

	magnitude = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (magnitude > 0.0)
	{
		vec.x /= magnitude;
		vec.y /= magnitude;
		vec.z /= magnitude;
		return (vec);
	}
	*error = 1;
	return ((t_vec3){0, 0, 0});
}

t_vec3	parse_orientation(char *str, int *error)
{
	char	**coords;
	t_vec3	vec;

	coords = ft_split(str, ',');
	if (!coords || !coords[0] || !coords[1] || !coords[2])
	{
		*error = 1;
		ft_free_split(coords);
		return ((t_vec3){0, 0, 0});
	}
	vec.x = parse_double(coords[0], -1.0, 1.0, error);
	vec.y = parse_double(coords[1], -1.0, 1.0, error);
	vec.z = parse_double(coords[2], -1.0, 1.0, error);
	ft_free_split(coords);
	if (*error)
		return ((t_vec3){0, 0, 0});
	return (normalize_vector(vec, error));
}
