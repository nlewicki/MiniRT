/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lkubler <lkubler@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:29:35 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/21 13:59:03 by lkubler          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

void	exit_mini(t_miniRT *mini)
{
	ft_putendl_fd("Exiting...", 1);
	if (mini->img)
		mlx_delete_image(mini->mlx, mini->img);
	if (mini->mlx)
		mlx_terminate(mini->mlx);
	exit(1);
}

void	key_hook(mlx_key_data_t key, void *param)
{
	t_miniRT	*mini;

	mini = (t_miniRT *)param;
	if (key.key == MLX_KEY_ESCAPE)
		exit_mini(mini);
	else
		return ;
}

void draw_smth(t_miniRT *mini)
{
	mlx_put_string(mini->mlx, "leo sucked", 50, 50);
}

void	loop(void *param)
{
	t_miniRT	*mini;

	mini = param;
	mlx_key_hook(mini->mlx, key_hook, mini);
}

int init_mlx(t_miniRT *mini)
{
	mlx_set_setting(MLX_MAXIMIZED, false);
	mini->mlx = mlx_init(WIDTH, HEIGHT, "mini", true);
	if (!mini->mlx)
		return (1);
	mini->img = mlx_new_image(mini->mlx, WIDTH, HEIGHT);
	if (!mini->img
		|| (mlx_image_to_window(mini->mlx, mini->img, 0, 0) < 0))
		return (2);
	return (0);
}

void leaks(void)
{
	system("leaks -q miniRT");
}

int main(void)
{
	atexit(leaks);
	t_miniRT	mini;
	mlx_image_t	*img;
	mlx_t		*mlx;
	
	mlx = mlx_init(WIDTH, HEIGHT, "mini_rt", true);
	if (!mlx)
		return (EXIT_FAILURE);
	img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img || mlx_image_to_window(mlx, img, 0, 0) < 0)
		return (EXIT_FAILURE);
	render_scene(img);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
