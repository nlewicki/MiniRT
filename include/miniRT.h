/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:14:07 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/27 16:54:26 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/get_next_line.h"
# include "../lib/libft/libft.h"
# include "fcntl.h"
# include "structs.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define WIDTH 800
# define HEIGHT 600
# define DEG2RAD 0.017453292519943295
# define REFLECTION 0

# define REFLECTION_MIN 0
# define REFLECTION_MAX 1
# define MAX_DEPTH 4

int			parse_rt_file(char *filename, t_scene *scene);
void		init_scene(t_scene *scene);
void		exit_error(char *msg);
void		free_scene(t_scene *scene);

void		ft_free_split(char **split);
double		parse_double(char *str, double min, double max, int *error);
t_color		parse_color(char *str, int *error);
t_vec3		parse_position(char *str, int *error);
t_vec3		parse_orientation(char *str, int *error);
t_vec3		normalize_vector(t_vec3 vec, int *error);

void		parse_ambient(char **tokens, t_scene *scene);
void		parse_camera(char **tokens, t_scene *scene);
void		parse_cylinder(char **tokens, t_scene *scene);
void		parse_light(char **tokens, t_scene *scene);
void		parse_plane(char **tokens, t_scene *scene);
void		parse_sphere(char **tokens, t_scene *scene);
void		parse_cone(char **tokens, t_scene *scene);

void		convert_objects(t_scene *scene);
double		hit_sphere(t_object *obj, const t_ray ray, t_hit *hit_info);
t_color		checkerboard_sphere(t_sphere *sph, t_vec3 point);
t_color		checkerboard_plane(t_plane *plane, t_vec3 point);
double		hit_plane(t_object *obj, const t_ray ray, t_hit *hit_info);
double		hit_cylinder(t_object *obj, const t_ray ray, t_hit *hit_info);
double		hit_cylinder_caps(t_cylinder_context *ctx, t_vec3 cap_center);
double		hit_cone(t_object *obj, const t_ray ray, t_hit *hit_info);
// t_color	compute_lighting(t_miniRT *mini, t_hit hit);
t_color		trace_ray(t_miniRT *mini, t_ray ray, int depth);

double		solve_quadratic(double a, double b, double c);

// utils
t_vec3		vec_sub(t_vec3 a, t_vec3 b);
t_vec3		vec_add(t_vec3 a, t_vec3 b);
double		vec_dot(t_vec3 a, t_vec3 b);
double		vec_skal(t_vec3 a, t_vec3 b);
t_vec3		vec_normalize(t_vec3 v);
t_vec3		vec_mul(t_vec3 v, double scalar);
t_vec3		vec_cross(t_vec3 a, t_vec3 b);
double		vec_length(t_vec3 v);
t_vec3		vec_neg(t_vec3 v);
t_vec3		vec_reflect(t_vec3 v, t_vec3 n);
t_color		color_mix(t_color a, t_color b, double factor);
t_color		color_scale(t_color c, double factor);
t_color		color_clamp(t_color c);
t_color		color_add(t_color c1, t_color c2);
uint32_t	color_to_uint32(t_color color);

t_color		checkerboard_cylinder(t_cylinder *cyl, t_vec3 point);
t_color		checkerboard_cone(t_cone *cone, t_vec3 point);

void		key_hook(mlx_key_data_t key, void *param);
void		exit_mini(t_miniRT *mini);

// render
void		render_scene(mlx_image_t *img, t_miniRT *mini);

bool		handle_material_property(t_miniRT *mini, int key);
bool		handle_checkerboard_toggle(t_miniRT *mini, int key);
bool		handle_camera_rotation(t_miniRT *mini, int key, t_vec3 forward);
void		handle_vertical_rotation(t_miniRT *mini, int key, t_vec3 forward);
void		handle_horizontal_rotation(t_miniRT *mini, int key, t_vec3 forward);
bool		handle_camera_movement(t_miniRT *mini, int key, t_vec3 forward);
t_color		trace_ray_skip_object(t_miniRT *mini, t_ray ray, int depth,
				t_object *skip_object);
t_color		compute_lighting_skip_object(t_miniRT *mini, t_hit hit,
				t_object *skip_object);

t_color		checkerboard_sphere(t_sphere *sph, t_vec3 point);
t_color		checkerboard_plane(t_plane *plane, t_vec3 point);
t_color		checkerboard_cylinder(t_cylinder *cyl, t_vec3 point);
t_color		checkerboard_cone(t_cone *cone, t_vec3 point);

bool		handle_sphere_reflection(t_sphere *sphere, double delta);
bool		handle_cylinder_reflection(t_cylinder *cylinder, double delta);
bool		handle_cone_reflection(t_cone *cone, double delta);

t_vec3		get_local_coords(t_vec3 point, t_vec3 origin);
void		set_base_hit_normal(t_hit *hit_info, t_vec3 normal, double denom);
void		check_mode(t_hit *hit_info, t_cone *cone, t_object *obj);

// Reflection functions
double		get_reflection_coef(t_object *hit_object);
t_ray		create_reflection_ray(t_hit hit, t_ray incident_ray);
t_hit		find_closest_hit_skip(t_miniRT *mini, t_ray ray,
				t_object *skip_object);
t_hit		find_closest_hit(t_miniRT *mini, t_ray ray, t_object **hit_object);
t_color		handle_reflection(t_miniRT *mini, t_hit hit, t_ray ray, int depth);
t_color		handle_reflection_skip(t_miniRT *mini, t_hit hit,
				t_reflection_context context);
double		calculate_light_factors(t_light_context ctx, t_light light);
double		compute_shadow_factor(t_miniRT *mini, t_vec3 point, t_light light,
				t_object *skip_object);
t_vec3		random_points(t_vec3 center, double radius);
bool		is_shadow_blocked(t_miniRT *mini, t_ray shadow_ray, double dist,
				t_object *skip_object);
bool		is_point_visible(t_miniRT *mini, t_vec3 point, t_light light,
				t_object *skip_object);
// camera
void		calculate_viewport(t_camera cam, int x, int y, double *coords);
void		calculate_camera_basis(t_camera cam, t_vec3 *basis);
t_ray		generate_camera_ray(t_camera cam, int x, int y);
t_color		compute_ambient_only(t_miniRT *mini, t_hit hit);
t_color		calculate_colored_light(t_hit hit, t_light light,
				double light_factor);
#endif
