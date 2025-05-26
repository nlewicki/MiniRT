/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolewicki <nicolewicki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:53:02 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/26 15:17:06 by nicolewicki      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "../lib/MLX42/include/MLX42/MLX42.h"

typedef struct s_color
{
	int					r;
	int					g;
	int					b;
	int					a;
}						t_color;

typedef struct s_vec3
{
	double				x;
	double				y;
	double				z;
}						t_vec3;

typedef struct s_ray
{
	t_vec3				origin;
	t_vec3				direction;
}						t_ray;

typedef enum e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	CONE
}						t_object_type;

typedef struct s_object	t_object;

typedef struct s_hit
{
	double				t;
	t_vec3				point;
	t_vec3				normal;
	t_color				color;
	t_object			*object;
}						t_hit;

typedef struct s_object
{
	t_object_type		type;
	void				*data;
	t_color				color;
	double				(*hit)(struct s_object *, const t_ray ray,
			t_hit *hit_info);
}						t_object;

typedef struct s_ambient
{
	double				ratio;
	t_color				color;
	int					is_set;
}						t_ambient;

typedef struct s_camera
{
	t_vec3				position;
	t_vec3				orientation;
	int					fov;
	int					is_set;
}						t_camera;

typedef struct s_light
{
	t_vec3				position;
	double				brightness;
	t_color				color;
}						t_light;

typedef struct s_sphere
{
	t_vec3				center;
	double				diameter;
	t_color				color;
	xpm_t				*texture;
	xpm_t				*bump_map;
	double				reflection;
	bool				checker;
	t_color				checker_black;
	t_color				checker_white;
}						t_sphere;

typedef struct s_plane
{
	t_vec3				position;
	t_vec3				orientation;
	t_color				color;
	xpm_t				*texture;
	xpm_t				*bump_map;
	double				reflection;
	bool				checker;
	t_color				checker_black;
	t_color				checker_white;
	double				limit_width;
	double				limit_height;
}						t_plane;

typedef struct s_cylinder
{
	t_vec3				position;
	t_vec3				orientation;
	double				diameter;
	double				height;
	t_color				color;
	xpm_t				*texture;
	xpm_t				*bump_map;
	double				reflection;
	bool				checker;
	t_color				checker_black;
	t_color				checker_white;
}						t_cylinder;

typedef struct s_cone
{
	t_vec3				apex;
	t_vec3				direction;
	double				angle;
	double				height;
	t_color				color;
	double				reflection;
	bool				checker;
	t_color				checker_black;
	t_color				checker_white;
}						t_cone;

typedef struct s_scene
{
	t_ambient			ambient;
	t_camera			camera;
	t_light				*lights;
	int					light_count;
	t_sphere			*spheres;
	int					sphere_count;
	t_plane				*planes;
	int					plane_count;
	t_cylinder			*cylinders;
	int					cylinder_count;
	t_cone				*cones;
	int					cone_count;
	t_object			*objects;
	int					object_count;
}						t_scene;

typedef struct s_miniRT
{
	mlx_t				*mlx;
	mlx_image_t			*img;
	t_scene				scene;
	bool				low_res_mode;
	int					res_scale;
	int					samples;
}						t_miniRT;

typedef struct s_hit_data
{
	t_hit		*hit_info;
	t_object	*obj;
	t_cone		*cone;
	t_ray		ray;
}				t_hit_data;

typedef struct s_reflection_context
{
	t_ray		ray;
	int			depth;
	t_object	*skip_object;
}				t_reflection_context;

typedef struct s_light_context
{
	t_miniRT	*mini;
	t_hit		hit;
	t_object	*skip_object;
	t_color		current_color;
}	t_light_context;

typedef struct s_render_context
{
	t_miniRT	*mini;
	mlx_image_t	*img;
	int			scale;
	int			x;
	int			y;
}	t_render_context;

typedef struct s_cylinder_context
{
	t_cylinder	*cyl;
	t_object	*obj;
	t_ray		ray;
	t_hit		*hit_info;
	double		closest_t;
}	t_cylinder_context;

typedef struct s_cylinder_math
{
	t_vec3		oc;
	t_vec3		axis;
	double		dot_d_axis;
	double		dot_oc_axis;
}	t_cylinder_math;

typedef struct s_cap_hit
{
	t_vec3	normal;
	t_vec3	point;
	double	t;
	double	denom;
}	t_cap_hit;

typedef struct s_sphere_intersection
{
	double		radius;
	t_vec3		oc;
	double		a;
	double		b;
	double		c;
	double		discriminant;
	double		t;
}	t_sphere_intersection;

typedef struct s_plane_intersection
{
	double		denom;
	t_vec3		hit_point;
	t_vec3		right;
	t_vec3		forward;
	double		x;
	double		z;
}	t_plane_intersection;

#endif
