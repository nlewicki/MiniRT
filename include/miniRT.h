/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/22 17:43:27 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
#include "fcntl.h"
# include "../lib/libft/libft.h"
# include "../lib/libft/get_next_line.h"
# include "../lib/MLX42/include/MLX42/MLX42.h"

# define WIDTH 800
# define HEIGHT 600
#define DEG2RAD (M_PI / 180.0)


// For RGB colors
typedef struct s_color
{
	int r;
	int g;
	int b;
	int a;
} t_color;

// For 3D vectors (used for positions and directions)
typedef struct s_vec3
{
	double x;
	double y;
	double z;
}	t_vec3;

typedef struct s_ray
{
	t_vec3 origin;
	t_vec3 direction;
}	t_ray;

typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER
} t_object_type;

typedef struct s_hit
{
	double t;        // Distance to hit point
	t_vec3 point;    // Hit point
	t_vec3 normal;   // Surface normal at hit point
	t_color color;   // Color at hit point
	// Add more fields if needed (e.g., for lighting)
} t_hit;

typedef struct s_object
{
	t_object_type type;
	void *data;
	t_color color;
	double (*hit)(struct s_object *, const t_ray ray, t_hit *hit_info);
} t_object;


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
	t_object *objects; // Array of objects (spheres, planes, etc.)
	int object_count;
} t_scene;

typedef struct s_miniRT
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_scene			scene;
}				t_miniRT;

int parse_rt_file(char *filename, t_scene *scene);
void	init_scene(t_scene *scene);
void	exit_error(char *msg);
void free_scene(t_scene *scene);

void 	ft_free_split(char **split);
double parse_double(char *str, double min, double max, int *error);
t_color	parse_color(char *str, int *error);
t_vec3 parse_position(char *str, int *error);
t_vec3 parse_orientation(char *str, int *error);

void parse_ambient(char **tokens, t_scene *scene);
void parse_camera(char **tokens, t_scene *scene);
void parse_cylinder(char **tokens, t_scene *scene);
void parse_light(char **tokens, t_scene *scene);
void parse_plane(char **tokens, t_scene *scene);
void parse_sphere(char **tokens, t_scene *scene);

void convert_objects(t_scene *scene);
double hit_sphere(t_object *obj, const t_ray ray, t_hit *hit_info);
double hit_plane(t_object *obj, const t_ray ray, t_hit *hit_info);
//double hit_cylinder(t_object *obj, const t_ray ray, t_hit *hit_info);
t_color compute_lighting(t_scene *scene, t_hit hit);

//utils
t_vec3 vec_sub(t_vec3 a, t_vec3 b);
t_vec3 vec_add(t_vec3 a, t_vec3 b);
double vec_skal(t_vec3 a, t_vec3 b);
t_vec3 vec_normalize(t_vec3 v);
t_vec3 vec_mul(t_vec3 v, double scalar);
t_vec3 vec_cross(t_vec3 a, t_vec3 b);
double vec_length(t_vec3 v);



//render
void render_scene(mlx_image_t *img, t_scene *scene);



#endif
