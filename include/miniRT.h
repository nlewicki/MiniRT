/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 11:39:55 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/02 13:11:11 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
#include "fcntl.h"
# include "../lib/libft/libft.h"
# include "../lib/libft/get_next_line.h"
# include "../lib/MLX42/include/MLX42/MLX42.h"

# define WIDTH 500
# define HEIGHT 500

// For RGB colors
typedef struct s_color
{
	int r;
	int g;
	int b;
} t_color;

// For 3D vectors (used for positions and directions)
typedef struct s_vec3
{
	double x;
	double y;
	double z;
}	t_vec3;

// Ambient lighting
typedef struct s_ambient
{
    double ratio;
    t_color color;
    int is_set;  // To check if it's already defined (since only one is allowed)
} t_ambient;

// Camera
typedef struct s_camera
{
    t_vec3 position;
    t_vec3 orientation;
    int fov;
    int is_set;  // To check if it's already defined
} t_camera;

// Light source
typedef struct s_light
{
    t_vec3 position;
    double brightness;
    t_color color;
} t_light;

// Sphere
typedef struct s_sphere
{
    t_vec3 center;
    double diameter;
    t_color color;
} t_sphere;

typedef struct s_plane
{
	t_vec3 position;
	t_vec3 orientation;
	t_color color;
} t_plane;

typedef struct s_cylinder
{
	t_vec3 position;
	t_vec3 orientation;
	double diameter;
	double height;
	t_color color;
} t_cylinder;

// Main scene struct
typedef struct s_scene
{
    t_ambient ambient;
    t_camera camera;
    t_light *lights;  // Array of lights
    int light_count;
    t_sphere *spheres; // Array of spheres
    int sphere_count;
	t_plane *planes; // Array of planes
	int plane_count;
	t_cylinder *cylinders; // Array of cylinders
	int cylinder_count;
} t_scene;

typedef struct s_miniRT
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_scene			scene;
}				t_miniRT;

int parse_rt_file(char *filename, t_scene *scene);

#endif
