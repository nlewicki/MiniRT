/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyhooks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:25:59 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/19 12:59:02 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static bool	print_keybind_legend(void)
{
	printf("\n=== MiniRT Keybindings ===\n");
	printf("Camera Controls:\n");
	printf("  WASD      - Move camera\n");
	printf("  Arrow Keys - Rotate camera\n");
	printf("\nCheckerboard Pattern:\n");
	printf("  Z - Toggle on spheres\n");
	printf("  X - Toggle on planes\n");
	printf("  C - Toggle on cylinders\n");
	printf("  V - Toggle on cones\n");
	printf("  B - Toggle on all objects\n");
	printf("\nMaterial Properties:\n");
	printf("  1/2 - Increase/Decrease KS\n");
	printf("  3/4 - Increase/Decrease Shine\n");
	printf("  5/6 - Increase/Decrease Reflection\n");
	printf("\nOther Controls:\n");
	printf("  F     - Toggle resolution mode\n");
	printf("  +/-   - Toggle sample count (2/128)\n");
	printf("  ESC   - Exit\n");
	printf("=====================\n\n");
	return (true);
}

static bool	handle_key_checks(mlx_key_data_t key, t_miniRT *mini)
{
	if (key.action != MLX_PRESS)
		return (false);
	if (key.key == MLX_KEY_ESCAPE)
		exit_mini(mini);
	if (key.key == MLX_KEY_F)
	{
		mini->low_res_mode = !mini->low_res_mode;
		render_scene(mini->img, mini);
		return (false);
	}
	mini->low_res_mode = true;
	return (true);
}

static bool	handle_samples(t_miniRT *mini, mlx_key_data_t key)
{
	if (key.key == MLX_KEY_EQUAL && mini->samples == 2)
	{
		mini->samples = 128;
		return (true);
	}
	else if (key.key == MLX_KEY_MINUS && mini->samples == 128)
	{
		mini->samples = 2;
		return (true);
	}
	return (false);
}

static bool	handle_key_actions(t_miniRT *mini, mlx_key_data_t key)
{
	t_vec3	forward;

	forward = vec_normalize(mini->scene.camera.orientation);
	if (key.key == MLX_KEY_W || key.key == MLX_KEY_A
		|| key.key == MLX_KEY_S || key.key == MLX_KEY_D)
		return (handle_camera_movement(mini, key.key, forward));
	if (key.key == MLX_KEY_LEFT || key.key == MLX_KEY_RIGHT
		|| key.key == MLX_KEY_UP || key.key == MLX_KEY_DOWN)
		return (handle_camera_rotation(mini, key.key, forward));
	if (key.key == MLX_KEY_1 || key.key == MLX_KEY_2)
		return (handle_material_property(mini, key.key));
	if (key.key == MLX_KEY_EQUAL || key.key == MLX_KEY_MINUS)
		return (handle_samples(mini, key));
	if (key.key == MLX_KEY_Z || key.key == MLX_KEY_X
		|| key.key == MLX_KEY_C || key.key == MLX_KEY_V || key.key == MLX_KEY_B)
		return (handle_checkerboard_toggle(mini, key.key));
	if (key.key == MLX_KEY_ESCAPE)
		exit_mini(mini);
	return (false);
}

void	key_hook(mlx_key_data_t key, void *param)
{
	static bool	legend_printed = false;
	t_miniRT	*mini;
	bool		needs_render;

	mini = (t_miniRT *)param;
	if (!legend_printed)
		legend_printed = print_keybind_legend();
	if (!handle_key_checks(key, mini))
		return ;
	needs_render = handle_key_actions(mini, key);
	if (needs_render)
		render_scene(mini->img, mini);
}
