/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:53:02 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/30 12:40:28 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "../lib/MLX42/include/MLX42/MLX42.h"

// For RGB colors
typedef struct s_color
{
	int				r;
	int				g;
	int				b;
	int				a;
}					t_color;

// For 3D vectors (used for positions and directions)
typedef struct s_vec3
{
	double			x;
	double			y;
	double			z;
}					t_vec3;

typedef struct s_ray
{
	t_vec3			origin;
	t_vec3			direction;
}					t_ray;

// Material properties for objects
typedef struct s_material
{
	t_color			color;       // Base color
	double			ks;          // Specular coefficient (0.0 - 1.0)
	double			shine;       // Shininess/phong exponent
	double			reflection;  // Reflection factor (0.0 - 1.0)
}					t_material;

typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER
}					t_object_type;

typedef struct s_object t_object;  // Forward declaration

typedef struct s_hit
{
	double t;             // Distance parameter
	t_vec3 point;         // Intersection point
	t_vec3 normal;        // Surface normal at intersection
	t_color color;        // Base color at hit point
	t_object *object;     // Pointer to the hit object (optional)
}            t_hit;

typedef struct s_miniRT t_miniRT;  // Forward declaration

typedef struct s_object
{
	t_object_type	type;
	void			*data;
	double			(*hit)(struct s_object *, const t_ray ray, t_hit *hit_info);
	t_material		*material;  // Material properties at hit point
	t_miniRT		*scene;      // Pointer back to the scene
}					t_object;

// Ambient lighting
typedef struct s_ambient
{
	double			ratio;
	t_color			color;
	int				is_set;
}					t_ambient;

// Camera
typedef struct s_camera
{
	t_vec3			position;
	t_vec3			orientation;
	int				fov;
	int				is_set;
}					t_camera;

// Light source
typedef struct s_light
{
	t_vec3			position;
	double			brightness;
	t_color			color;
}					t_light;

// Sphere
typedef struct s_sphere
{
	t_vec3			center;
	double			diameter;
	t_color			color;
}					t_sphere;

// Plane
typedef struct s_plane
{
	t_vec3			position;
	t_vec3			orientation;
	t_color			color;
}					t_plane;

// Cylinder
typedef struct s_cylinder
{
	t_vec3			position;
	t_vec3			orientation;
	double			diameter;
	double			height;
	t_color			color;
}					t_cylinder;

// Main scene struct
typedef struct s_scene
{
	t_ambient		ambient;
	t_camera		camera;
	t_light			*lights;     // Array of lights
	int				light_count;
	t_sphere		*spheres; // Array of spheres
    int				sphere_count;
    t_plane			*planes; // Array of planes
    int				plane_count;
    t_cylinder		*cylinders; // Array of cylinders
    int				cylinder_count;
	t_object		*objects;    // Array of objects (spheres, planes, etc.)
	int				object_count;
}					t_scene;

typedef struct s_miniRT
{
	mlx_t			*mlx;
	mlx_image_t		*img;
	t_scene			scene;
	bool			low_res_mode;
	int				res_scale;
	int				samples;
	// Default material values for new objects
	t_material		default_material;
}					t_miniRT;

# endif
