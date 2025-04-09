/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolewicki <nicolewicki@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:29:35 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/09 15:52:13 by nicolewicki      ###   ########.fr       */
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

int main(int argc, char **argv)
{
	atexit(leaks);
	t_miniRT	mini;
	int return_value = 0;

	if (argc != 2)
		exit_error("Usage: ./miniRT <filename>\n");
	// return_value = init_mlx(&mini);
	// if (return_value)
	// 	return (return_value);
	init_scene(&mini.scene);
	return_value = parse_rt_file(argv[1], &mini.scene);
	// draw_smth(&mini);
	// mlx_loop_hook(mini.mlx, loop, &mini);
	// mlx_loop(mini.mlx);
	printf("Cylinder: %i\n", mini.scene.cylinder_count);
	printf("Cylinder added: position=(%.2f, %.2f, %.2f), orientation=(%.2f, %.2f, %.2f), diameter=%.2f, height=%.2f, color=(%d, %d, %d)\n",
           mini.scene.cylinders->position.x, mini.scene.cylinders->position.y, mini.scene.cylinders->position.z,
           mini.scene.cylinders->orientation.x, mini.scene.cylinders->orientation.y, mini.scene.cylinders->orientation.z,
           mini.scene.cylinders->diameter, mini.scene.cylinders->height,
           mini.scene.cylinders->color.r, mini.scene.cylinders->color.g, mini.scene.cylinders->color.b); // test ob alles in mini struct ist
	free_scene(&mini.scene);
	return (return_value);
}
