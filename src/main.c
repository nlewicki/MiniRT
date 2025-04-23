/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leokubler <leokubler@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:29:35 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/23 16:15:22 by leokubler        ###   ########.fr       */
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

//gecuttete version von key_hook

// static void	handle_camera_movement(t_miniRT *mini, int key, t_vec3 forward)
// {
// 	t_vec3 up = {0, 1, 0};  // World up vector
// 	t_vec3 right;
// 	const double move_speed = 0.5;

// 	right = vec_cross(forward, up);
// 	right = vec_normalize(right);

// 	if (key == MLX_KEY_W)
// 		mini->scene.camera.position = vec_add(mini->scene.camera.position, vec_mul(forward, move_speed));
// 	else if (key == MLX_KEY_S)
// 		mini->scene.camera.position = vec_sub(mini->scene.camera.position, vec_mul(forward, move_speed));
// 	else if (key == MLX_KEY_A)
// 		mini->scene.camera.position = vec_sub(mini->scene.camera.position, vec_mul(right, move_speed));
// 	else if (key == MLX_KEY_D)
// 		mini->scene.camera.position = vec_add(mini->scene.camera.position, vec_mul(right, move_speed));
// }

// static void	handle_horizontal_rotation(t_miniRT *mini, int key, t_vec3 forward)
// {
// 	const double rot_speed = 0.1;
// 	t_vec3 rotated = forward;

// 	if (key == MLX_KEY_RIGHT)
// 	{
// 		rotated.x = forward.x * cos(rot_speed) - forward.z * sin(rot_speed);
// 		rotated.z = forward.x * sin(rot_speed) + forward.z * cos(rot_speed);
// 	}
// 	else if (key == MLX_KEY_LEFT)
// 	{
// 		rotated.x = forward.x * cos(-rot_speed) - forward.z * sin(-rot_speed);
// 		rotated.z = forward.x * sin(-rot_speed) + forward.z * cos(-rot_speed);
// 	}
// 	mini->scene.camera.orientation = vec_normalize(rotated);
// }

// static void	handle_vertical_rotation(t_miniRT *mini, int key, t_vec3 forward)
// {
// 	const double rot_speed = 0.1;
// 	t_vec3 rotated = forward;

// 	if (key == MLX_KEY_UP)
// 	{
// 		rotated.y = forward.y * cos(-rot_speed) - forward.z * sin(-rot_speed);
// 		rotated.z = forward.y * sin(-rot_speed) + forward.z * cos(-rot_speed);
// 	}
// 	else if (key == MLX_KEY_DOWN)
// 	{
// 		rotated.y = forward.y * cos(rot_speed) - forward.z * sin(rot_speed);
// 		rotated.z = forward.y * sin(rot_speed) + forward.z * cos(rot_speed);
// 	}
// 	mini->scene.camera.orientation = vec_normalize(rotated);
// }

// static void	handle_camera_rotation(t_miniRT *mini, int key, t_vec3 forward)
// {
// 	if (key == MLX_KEY_LEFT || key == MLX_KEY_RIGHT)
// 		handle_horizontal_rotation(mini, key, forward);
// 	else if (key == MLX_KEY_UP || key == MLX_KEY_DOWN)
// 		handle_vertical_rotation(mini, key, forward);
// }

// static void	handle_resolution_mode(t_miniRT *mini)
// {
// 	mini->low_res_mode = !mini->low_res_mode;
// 	render_scene(mini->img, mini);
// }

// static bool	handle_key_checks(mlx_key_data_t key, t_miniRT *mini)
// {
// 	if (key.key == MLX_KEY_ESCAPE)
// 		exit_mini(mini);
// 	if (key.action != MLX_PRESS)
// 		return (false);
// 	if (key.key == MLX_KEY_F)
// 	{
// 		handle_resolution_mode(mini);
// 		return (false);
// 	}
// 	mini->low_res_mode = true;
// 	return (true);
// }

// void	key_hook(mlx_key_data_t key, void *param)
// {
// 	t_miniRT	*mini;
// 	t_vec3		forward;
// 	bool		needs_render;

// 	mini = (t_miniRT *)param;
// 	if (!handle_key_checks(key, mini))
// 		return;
// 	forward = vec_normalize(mini->scene.camera.orientation);
// 	needs_render = false;
// 	if (key.key == MLX_KEY_W || key.key == MLX_KEY_A ||
// 		key.key == MLX_KEY_S || key.key == MLX_KEY_D)
// 	{
// 		handle_camera_movement(mini, key.key, forward);
// 		needs_render = true;
// 	}
// 	else if (key.key == MLX_KEY_LEFT || key.key == MLX_KEY_RIGHT ||
// 		key.key == MLX_KEY_UP || key.key == MLX_KEY_DOWN)
// 	{
// 		handle_camera_rotation(mini, key.key, forward);
// 		needs_render = true;
// 	}
// 	if (needs_render)
// 		render_scene(mini->img, mini);
// }
void	key_hook(mlx_key_data_t key, void *param)
{
	t_miniRT	*mini;
	t_vec3 right;
	t_vec3 up = {0, 1, 0};  // World up vector
	const double move_speed = 0.5;
	const double rot_speed = 0.1;  // Rotation speed in radians

	mini = (t_miniRT *)param;
	t_vec3 forward = vec_normalize(mini->scene.camera.orientation);
	if (key.key == MLX_KEY_ESCAPE)
		exit_mini(mini);
	// Only handle key press events (not release or repeat)
	if (key.action != MLX_PRESS)
		return;

	// F key toggles between low and high resolution rendering
	if (key.key == MLX_KEY_F)
	{
		mini->low_res_mode = !mini->low_res_mode;
		render_scene(mini->img, mini);
		return;
	}

	// Enable low res mode when starting movement
	mini->low_res_mode = true;

	// Calculate right vector as cross product of forward and up
	right = vec_cross(forward, up);
	right = vec_normalize(right);

	if (key.key == MLX_KEY_W)
		mini->scene.camera.position = vec_add(mini->scene.camera.position, vec_mul(forward, move_speed));
	else if (key.key == MLX_KEY_S)
		mini->scene.camera.position = vec_sub(mini->scene.camera.position, vec_mul(forward, move_speed));
	else if (key.key == MLX_KEY_A)
		mini->scene.camera.position = vec_sub(mini->scene.camera.position, vec_mul(right, move_speed));
	else if (key.key == MLX_KEY_D)
		mini->scene.camera.position = vec_add(mini->scene.camera.position, vec_mul(right, move_speed));
	// Rotate camera with arrow keys
	else if (key.key == MLX_KEY_RIGHT)
	{
		// Rotate around the up vector (left)
		t_vec3 rotated = forward;
		rotated.x = forward.x * cos(rot_speed) - forward.z * sin(rot_speed);
		rotated.z = forward.x * sin(rot_speed) + forward.z * cos(rot_speed);
		mini->scene.camera.orientation = vec_normalize(rotated);
	}
	else if (key.key == MLX_KEY_LEFT)
	{
		// Rotate around the up vector (right)
		t_vec3 rotated = forward;
		rotated.x = forward.x * cos(-rot_speed) - forward.z * sin(-rot_speed);
		rotated.z = forward.x * sin(-rot_speed) + forward.z * cos(-rot_speed);
		mini->scene.camera.orientation = vec_normalize(rotated);
	}
	else if (key.key == MLX_KEY_UP)
	{
		// Rotate around the right vector (up)
		t_vec3 rotated = forward;
		rotated.y = forward.y * cos(-rot_speed) - forward.z * sin(-rot_speed);
		rotated.z = forward.y * sin(-rot_speed) + forward.z * cos(-rot_speed);
		mini->scene.camera.orientation = vec_normalize(rotated);
	}
	else if (key.key == MLX_KEY_DOWN)
	{
		// Rotate around the right vector (down)
		t_vec3 rotated = forward;
		rotated.y = forward.y * cos(rot_speed) - forward.z * sin(rot_speed);
		rotated.z = forward.y * sin(rot_speed) + forward.z * cos(rot_speed);
		mini->scene.camera.orientation = vec_normalize(rotated);
	}
	else
		return;

	// Re-render the scene after camera movement
	render_scene(mini->img, mini);
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
	mini->low_res_mode = true;  // Start in low res mode
	mini->res_scale = 4;        // 1/4 resolution while moving
	return (0);
}

void leaks(void)
{
	system("leaks -q miniRT");
}

void print_objects(t_scene *scene)
{
    int i;

    printf("Total objects: %d\n", scene->object_count);
    for (i = 0; i < scene->object_count; i++)
    {
        t_object *obj = &scene->objects[i];
        printf("Object #%d:\n", i + 1);
        printf("  Type: ");
        if (obj->type == SPHERE)
        {
            t_sphere *sphere = (t_sphere *)obj->data;
            printf("Sphere\n");
            printf("  Center: (%.2f, %.2f, %.2f)\n", sphere->center.x, sphere->center.y, sphere->center.z);
            printf("  Diameter: %.2f\n", sphere->diameter);
            printf("  Color: (%d, %d, %d, %d)\n", sphere->color.r, sphere->color.g, sphere->color.b, sphere->color.a);
        }
        else if (obj->type == PLANE)
        {
            t_plane *plane = (t_plane *)obj->data;
            printf("Plane\n");
            printf("  Position: (%.2f, %.2f, %.2f)\n", plane->position.x, plane->position.y, plane->position.z);
            printf("  Orientation: (%.2f, %.2f, %.2f)\n", plane->orientation.x, plane->orientation.y, plane->orientation.z);
            printf("  Color: (%d, %d, %d, %d)\n", plane->color.r, plane->color.g, plane->color.b, plane->color.a);
        }
        else if (obj->type == CYLINDER)
        {
            t_cylinder *cylinder = (t_cylinder *)obj->data;
            printf("Cylinder\n");
            printf("  Position: (%.2f, %.2f, %.2f)\n", cylinder->position.x, cylinder->position.y, cylinder->position.z);
            printf("  Orientation: (%.2f, %.2f, %.2f)\n", cylinder->orientation.x, cylinder->orientation.y, cylinder->orientation.z);
            printf("  Diameter: %.2f\n", cylinder->diameter);
            printf("  Height: %.2f\n", cylinder->height);
            printf("  Color: (%d, %d, %d, %d)\n", cylinder->color.r, cylinder->color.g, cylinder->color.b, cylinder->color.a);
        }
        else
        {
            printf("Unknown\n");
        }
        printf("\n");
    }
}

int	main(int argc, char **argv)
{
	atexit(leaks);
	t_miniRT	mini;
	int			return_value = 0;

	if (argc != 2)
		exit_error("Usage: ./miniRT <filename>\n");
	return_value = init_mlx(&mini);
	if (return_value)
		return (return_value);
	init_scene(&mini.scene);
	return_value = parse_rt_file(argv[1], &mini.scene);
	convert_objects(&mini.scene);
	print_objects(&mini.scene);
	if (return_value)
		return (return_value);
	printf("----------------------\n");
	render_scene(mini.img, &mini);
	printf("----------------------\n");
	mlx_loop_hook(mini.mlx, loop, &mini); // optional
	mlx_loop(mini.mlx);
	free_scene(&mini.scene);
	mlx_terminate(mini.mlx);
	return (return_value);
}

