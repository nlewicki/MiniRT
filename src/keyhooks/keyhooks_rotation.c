/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyhooks_rotation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 11:49:24 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/13 11:51:14 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

void	handle_camera_movement(t_miniRT *mini, int key, t_vec3 forward)
{
	const double	move_speed = 0.5;
	t_vec3			up;
	t_vec3			right;

	up = (t_vec3){0, 1, 0};
	right = vec_cross(forward, up);
	right = vec_normalize(right);
	if (key == MLX_KEY_W)
		mini->scene.camera.position = vec_add(mini->scene.camera.position, vec_mul(forward, move_speed));
	else if (key == MLX_KEY_S)
		mini->scene.camera.position = vec_sub(mini->scene.camera.position, vec_mul(forward, move_speed));
	else if (key == MLX_KEY_A)
		mini->scene.camera.position = vec_sub(mini->scene.camera.position, vec_mul(right, move_speed));
	else if (key == MLX_KEY_D)
		mini->scene.camera.position = vec_add(mini->scene.camera.position, vec_mul(right, move_speed));
}

void	handle_horizontal_rotation(t_miniRT *mini, int key, t_vec3 forward)
{
	const double	rot_speed = 0.1;
	t_vec3			rotated;

	rotated = forward;
	if (key == MLX_KEY_RIGHT)
	{
		rotated.x = forward.x * cos(rot_speed) - forward.z * sin(rot_speed);
		rotated.z = forward.x * sin(rot_speed) + forward.z * cos(rot_speed);
	}
	else if (key == MLX_KEY_LEFT)
	{
		rotated.x = forward.x * cos(-rot_speed) - forward.z * sin(-rot_speed);
		rotated.z = forward.x * sin(-rot_speed) + forward.z * cos(-rot_speed);
	}
	mini->scene.camera.orientation = vec_normalize(rotated);
}

void	handle_vertical_rotation(t_miniRT *mini, int key, t_vec3 forward)
{
	const double	rot_speed = 0.1;
	t_vec3			rotated;

	rotated = forward;
	if (key == MLX_KEY_UP)
	{
		rotated.y = forward.y * cos(-rot_speed) - forward.z * sin(-rot_speed);
		rotated.z = forward.y * sin(-rot_speed) + forward.z * cos(-rot_speed);
	}
	else if (key == MLX_KEY_DOWN)
	{
		rotated.y = forward.y * cos(rot_speed) - forward.z * sin(rot_speed);
		rotated.z = forward.y * sin(rot_speed) + forward.z * cos(rot_speed);
	}
	mini->scene.camera.orientation = vec_normalize(rotated);
}

void	handle_camera_rotation(t_miniRT *mini, int key, t_vec3 forward)
{
	if (key == MLX_KEY_LEFT || key == MLX_KEY_RIGHT)
		handle_horizontal_rotation(mini, key, forward);
	else if (key == MLX_KEY_UP || key == MLX_KEY_DOWN)
		handle_vertical_rotation(mini, key, forward);
}
