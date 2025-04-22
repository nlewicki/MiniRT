/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:29:35 by nlewicki          #+#    #+#             */
/*   Updated: 2025/04/22 14:30:12 by nlewicki         ###   ########.fr       */
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

int main(int argc, char **argv)
{
	atexit(leaks);
	t_miniRT	mini;
	int return_value = 0;

	if (argc != 2)
		exit_error("Usage: ./miniRT <filename>\n");
	 return_value = init_mlx(&mini);
	 if (return_value)
	 	return (return_value);
	init_scene(&mini.scene);
	return_value = parse_rt_file(argv[1], &mini.scene);
	convert_objects(&mini.scene);
	print_objects(&mini.scene);
	 draw_smth(&mini);
	 mlx_loop_hook(mini.mlx, loop, &mini);
	 mlx_loop(mini.mlx);
	free_scene(&mini.scene);
	return (return_value);
}
