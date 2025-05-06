/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/05/06 11:41:09 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../lib/MLX42/include/MLX42/MLX42.h"
# include "../lib/libft/get_next_line.h"
# include "../lib/libft/libft.h"
# include "structs.h"
# include "fcntl.h"
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define WIDTH 800
# define HEIGHT 600
# define DEG2RAD (M_PI / 180.0)
# define SHINE 0
# define KS 0
# define REFLECTION 0

# define SHINE_MIN 0
# define SHINE_MAX 100
# define KS_MIN 0
# define KS_MAX 1
# define REFLECTION_MIN 0
# define REFLECTION_MAX 1
# define MAX_DEPTH 4

int					parse_rt_file(char *filename, t_scene *scene);
void				init_scene(t_scene *scene);
void				exit_error(char *msg);
void				free_scene(t_scene *scene);

void				ft_free_split(char **split);
double				parse_double(char *str, double min, double max, int *error);
t_color				parse_color(char *str, int *error);
t_vec3				parse_position(char *str, int *error);
t_vec3				parse_orientation(char *str, int *error);

void				parse_ambient(char **tokens, t_scene *scene);
void				parse_camera(char **tokens, t_scene *scene);
void				parse_cylinder(char **tokens, t_scene *scene);
void				parse_light(char **tokens, t_scene *scene);
void				parse_plane(char **tokens, t_scene *scene);
void				parse_sphere(char **tokens, t_scene *scene);

void				convert_objects(t_scene *scene);
double				hit_sphere(t_object *obj, const t_ray ray, t_hit *hit_info);
t_color				checkerboard_sphere(t_sphere *sph, t_vec3 point);
t_color				checkerboard_plane(t_plane *plane, t_vec3 point);
double				hit_plane(t_object *obj, const t_ray ray, t_hit *hit_info);
double				hit_cylinder(t_object *obj, const t_ray ray, t_hit *hit_info);
t_color				compute_lighting(t_miniRT *mini, t_hit hit);
t_color trace_ray(t_miniRT *mini, t_ray ray, int depth);

// utils
t_vec3				vec_sub(t_vec3 a, t_vec3 b);
t_vec3				vec_add(t_vec3 a, t_vec3 b);
double				vec_dot(t_vec3 a, t_vec3 b);
double				vec_skal(t_vec3 a, t_vec3 b);
t_vec3				vec_normalize(t_vec3 v);
t_vec3				vec_mul(t_vec3 v, double scalar);
t_vec3				vec_cross(t_vec3 a, t_vec3 b);
double				vec_length(t_vec3 v);
t_vec3 vec_neg(t_vec3 v);
t_vec3 vec_reflect(t_vec3 v, t_vec3 n);
t_color color_mix(t_color a, t_color b, double factor);
t_color color_scale(t_color c, double factor);
t_color color_clamp(t_color c);

void	key_hook(mlx_key_data_t key, void *param);
void	exit_mini(t_miniRT *mini);

// render
void				render_scene(mlx_image_t *img, t_miniRT *mini);

#endif
