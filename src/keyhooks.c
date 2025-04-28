/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyhooks.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 10:25:59 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/28 11:15:28 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

//gecuttete version von key_hook

static void	handle_camera_movement(t_miniRT *mini, int key, t_vec3 forward)
{
	t_vec3 up = {0, 1, 0};  // World up vector
	t_vec3 right;
	const double move_speed = 0.5;

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

static void	handle_horizontal_rotation(t_miniRT *mini, int key, t_vec3 forward)
{
	const double rot_speed = 0.1;
	t_vec3 rotated = forward;

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

static void	handle_vertical_rotation(t_miniRT *mini, int key, t_vec3 forward)
{
	const double rot_speed = 0.1;
	t_vec3 rotated = forward;

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

static void	handle_camera_rotation(t_miniRT *mini, int key, t_vec3 forward)
{
	if (key == MLX_KEY_LEFT || key == MLX_KEY_RIGHT)
		handle_horizontal_rotation(mini, key, forward);
	else if (key == MLX_KEY_UP || key == MLX_KEY_DOWN)
		handle_vertical_rotation(mini, key, forward);
}

static void	handle_resolution_mode(t_miniRT *mini)
{
	mini->low_res_mode = !mini->low_res_mode; //swapped von true zu false und umgekehrt
	render_scene(mini->img, mini);
}

static bool	handle_key_checks(mlx_key_data_t key, t_miniRT *mini)
{
	if (key.action != MLX_PRESS)
		return (false);
	if (key.key == MLX_KEY_ESCAPE)
		exit_mini(mini);
	if (key.key == MLX_KEY_F)
	{
		handle_resolution_mode(mini);
		return (false);
	}
	mini->low_res_mode = true;
	return (true);
}

static bool adjust_material_property(t_scene *scene, int property_type, double delta)
{
	bool changed = false;
	for (int i = 0; i < scene->object_count; i++)
	{
		t_object *obj = &scene->objects[i];
		if (obj->type == SPHERE)
		{
			t_sphere *sphere = (t_sphere *)obj->data;
			double new_value;

			if (property_type == 0)  // KS
			{
				new_value = fmax(KS_MIN, fmin(KS_MAX, sphere->ks + delta));
				if (new_value != sphere->ks)
				{
					sphere->ks = new_value;
					changed = true;
					printf("OBJECT TYPE: SPHERE\nKS: %f\n", sphere->ks);
				}
			}
			else if (property_type == 1)  // SHINE
			{
				new_value = fmax(SHINE_MIN, fmin(SHINE_MAX, sphere->shine + delta));
				if (new_value != sphere->shine)
				{
					sphere->shine = new_value;
					changed = true;
					printf("OBJECT TYPE: SPHERE\nSHINE: %f\n", sphere->shine);
				}
			}
			else if (property_type == 2)  // REFLECTION
			{
				new_value = fmax(REFLECTION_MIN, fmin(REFLECTION_MAX, sphere->reflection + delta));
				if (new_value != sphere->reflection)
				{
					sphere->reflection = new_value;
					changed = true;
					printf("OBJECT TYPE: SPHERE\nREFLECTION: %f\n", sphere->reflection);
				}
			}
		}
	}
	return changed;
}

static bool handle_material_property(t_miniRT *mini, int key)
{
	const double ks_step = 0.1;    // Step size for KS
	const double shine_step = 5.0; // Step size for SHINE
	const double reflection_step = 0.1; // Step size for REFLECTION

	if (key == MLX_KEY_1)
		return adjust_material_property(&mini->scene, 0, ks_step);  // Increase KS
	else if (key == MLX_KEY_2)
		return adjust_material_property(&mini->scene, 0, -ks_step); // Decrease KS
	else if (key == MLX_KEY_3)
		return adjust_material_property(&mini->scene, 1, shine_step);  // Increase SHINE
	else if (key == MLX_KEY_4)
		return adjust_material_property(&mini->scene, 1, -shine_step); // Decrease SHINE
	else if (key == MLX_KEY_5)
		return adjust_material_property(&mini->scene, 2, reflection_step);  // Increase REFLECTION
	else if (key == MLX_KEY_6)
		return adjust_material_property(&mini->scene, 2, -reflection_step); // Decrease REFLECTION
	return false;
}

static bool handle_samples(t_miniRT *mini, mlx_key_data_t key)
{
	if (key.key == MLX_KEY_EQUAL && mini->samples == 2)
	{
		mini->samples = 128;
		return true;
	}
	else if (key.key == MLX_KEY_MINUS && mini->samples == 128)
	{
		mini->samples = 2;
		return true;
	}
	return false;
}

void	key_hook(mlx_key_data_t key, void *param)
{
	t_miniRT	*mini;
	t_vec3		forward;
	bool		needs_render;

	mini = (t_miniRT *)param;
	if (!handle_key_checks(key, mini))
		return;
	forward = vec_normalize(mini->scene.camera.orientation);
	needs_render = false;
	if (key.key == MLX_KEY_W || key.key == MLX_KEY_A ||
		key.key == MLX_KEY_S || key.key == MLX_KEY_D)
	{
		handle_camera_movement(mini, key.key, forward);
		needs_render = true;
	}
	else if (key.key == MLX_KEY_LEFT || key.key == MLX_KEY_RIGHT ||
		key.key == MLX_KEY_UP || key.key == MLX_KEY_DOWN)
	{
		handle_camera_rotation(mini, key.key, forward);
		needs_render = true;
	}
	else if (key.key == MLX_KEY_1 || key.key == MLX_KEY_2 ||
		key.key == MLX_KEY_3 || key.key == MLX_KEY_4 ||
		key.key == MLX_KEY_5 || key.key == MLX_KEY_6)
	{
		needs_render = handle_material_property(mini, key.key);
	}
	else if (key.key == MLX_KEY_EQUAL || key.key == MLX_KEY_MINUS)
	{
		needs_render = handle_samples(mini, key);
	}
	if (needs_render)
		render_scene(mini->img, mini);
}
