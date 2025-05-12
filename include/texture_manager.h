/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_manager.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:57:00 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/12 11:58:07 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_MANAGER_H
# define TEXTURE_MANAGER_H

# include "structs.h"
# include "../lib/MLX42/include/MLX42/MLX42.h"

t_color sample_texture(t_texture *tex, double u, double v);
void init_texture(t_texture *tex);
bool load_texture(t_texture *tex, const char *path, mlx_t *mlx);
t_color get_texture_color(t_texture *tex, t_vec3 point, t_object *obj);
void cleanup_texture(t_texture *tex);

#endif
