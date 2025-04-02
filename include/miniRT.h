/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 11:39:55 by nlewicki          #+#    #+#             */
/*   Updated: 2025/03/26 12:28:19 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../lib/libft/libft.h"
# include "../lib/libft/get_next_line.h"
# include "../lib/MLX42/include/MLX42/MLX42.h"

# define WIDTH 500
# define HEIGHT 500

typedef struct s_miniRT
{
	mlx_t			*mlx;
	mlx_image_t		*img;
}				t_miniRT;

#endif
