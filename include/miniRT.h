/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 12:14:07 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/16 11:53:42 by nlewicki         ###   ########.fr       */
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
# include <unistd.h>

# define WIDTH 800
# define HEIGHT 600
# define DEG2RAD (M_PI / 180.0)
# define REFLECTION 0

# define REFLECTION_MIN 0
# define REFLECTION_MAX 1
# define MAX_DEPTH 4

int		parse_rt_file(char *filename, t_scene *scene);
void	init_scene(t_scene *scene);
void	exit_error(char *msg);
void	free_scene(t_scene *scene);

void	ft_free_split(char **split);
double	parse_double(char *str, double min, double max, int *error);
t_color	parse_color(char *str, int *error);
t_vec3	parse_position(char *str, int *error);
t_vec3	parse_orientation(char *str, int *error);

void	parse_ambient(char **tokens, t_scene *scene);
void	parse_camera(char **tokens, t_scene *scene);
void	parse_cylinder(char **tokens, t_scene *scene);
void	parse_light(char **tokens, t_scene *scene);
void	parse_plane(char **tokens, t_scene *scene);
void	parse_sphere(char **tokens, t_scene *scene);
void	parse_cone(char **tokens, t_scene *scene);

void	convert_objects(t_scene *scene);
double	hit_sphere(t_object *obj, const t_ray ray, t_hit *hit_info);
t_color	checkerboard_sphere(t_sphere *sph, t_vec3 point);
t_color	checkerboard_plane(t_plane *plane, t_vec3 point);
double	hit_plane(t_object *obj, const t_ray ray, t_hit *hit_info);
double	hit_cylinder(t_object *obj, const t_ray ray, t_hit *hit_info);
double	hit_cone(t_object *obj, const t_ray ray, t_hit *hit_info);
t_color	compute_lighting(t_miniRT *mini, t_hit hit);
t_color	trace_ray(t_miniRT *mini, t_ray ray, int depth);

double	solve_quadratic(double a, double b, double c);

// utils
t_vec3	vec_sub(t_vec3 a, t_vec3 b);
t_vec3	vec_add(t_vec3 a, t_vec3 b);
double	vec_dot(t_vec3 a, t_vec3 b);
double	vec_skal(t_vec3 a, t_vec3 b);
t_vec3	vec_normalize(t_vec3 v);
t_vec3	vec_mul(t_vec3 v, double scalar);
t_vec3	vec_cross(t_vec3 a, t_vec3 b);
double	vec_length(t_vec3 v);
t_vec3	vec_neg(t_vec3 v);
t_vec3	vec_reflect(t_vec3 v, t_vec3 n);
t_color	color_mix(t_color a, t_color b, double factor);
t_color	color_scale(t_color c, double factor);
t_color	color_clamp(t_color c);

t_color	checkerboard_cylinder(t_cylinder *cyl, t_vec3 point);
t_color	checkerboard_cone(t_cone *cone, t_vec3 point);

void	key_hook(mlx_key_data_t key, void *param);
void	exit_mini(t_miniRT *mini);

// render
void	render_scene(mlx_image_t *img, t_miniRT *mini);

bool	handle_material_property(t_miniRT *mini, int key);
bool	handle_checkerboard_toggle(t_miniRT *mini, int key);
void	handle_camera_rotation(t_miniRT *mini, int key, t_vec3 forward);
void	handle_vertical_rotation(t_miniRT *mini, int key, t_vec3 forward);
void	handle_horizontal_rotation(t_miniRT *mini, int key, t_vec3 forward);
void	handle_camera_movement(t_miniRT *mini, int key, t_vec3 forward);
t_color trace_ray_skip_object(t_miniRT *mini, t_ray ray, int depth, t_object *skip_object);
t_color				compute_lighting_skip_object(t_miniRT *mini, t_hit hit, t_object *skip_object);;


t_color	checkerboard_sphere(t_sphere *sph, t_vec3 point);
t_color	checkerboard_plane(t_plane *plane, t_vec3 point);
t_color	checkerboard_cylinder(t_cylinder *cyl, t_vec3 point);
t_color	checkerboard_cone(t_cone *cone, t_vec3 point);

#endif
