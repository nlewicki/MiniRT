/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 11:29:35 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/28 11:52:41 by nlewicki         ###   ########.fr       */
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

void	loop(void *param)
{
	t_miniRT	*mini;

	mini = param;
	mlx_key_hook(mini->mlx, key_hook, mini);
}

int	init_mlx(t_miniRT *mini)
{
	mlx_set_setting(MLX_MAXIMIZED, false);
	mini->mlx = mlx_init(WIDTH, HEIGHT, "mini", false);
	if (!mini->mlx)
		return (1);
	mini->img = mlx_new_image(mini->mlx, WIDTH, HEIGHT);
	if (!mini->img || (mlx_image_to_window(mini->mlx, mini->img, 0, 0) < 0))
		return (2);
	mini->low_res_mode = true;
	mini->res_scale = 4;
	mini->samples = 2;
	return (0);
}

// void	leaks(void)
// {
// 	system("leaks -q miniRT");
// }

// void	print_objects(t_scene *scene)
// {
// 	int			i;
// 	t_object	*obj;
// 	t_sphere	*sphere;
// 	t_plane		*plane;
// 	t_cylinder	*cylinder;

// 	printf("Total objects: %d\n", scene->object_count);
// 	for (i = 0; i < scene->object_count; i++)
// 	{
// 		obj = &scene->objects[i];
// 		printf("Object #%d:\n", i + 1);
// 		printf("  Type: ");
// 		if (obj->type == SPHERE)
// 		{
// 			sphere = (t_sphere *)obj->data;
// 			printf("Sphere\n");
// 			printf("  Center: (%.2f, %.2f, %.2f)\n", sphere->center.x,
// 				sphere->center.y, sphere->center.z);
// 			printf("  Diameter: %.2f\n", sphere->diameter);
// 			printf("  Color: (%d, %d, %d, %d)\n", sphere->color.r,
// 				sphere->color.g, sphere->color.b, sphere->color.a);
// 		}
// 		else if (obj->type == PLANE)
// 		{
// 			plane = (t_plane *)obj->data;
// 			printf("Plane\n");
// 			printf("  Position: (%.2f, %.2f, %.2f)\n", plane->position.x,
// 				plane->position.y, plane->position.z);
// 			printf("  Orientation: (%.2f, %.2f, %.2f)\n", plane->orientation.x,
// 				plane->orientation.y, plane->orientation.z);
// 			printf("  Color: (%d, %d, %d, %d)\n", plane->color.r,
// 				plane->color.g, plane->color.b, plane->color.a);
// 			if (plane->limit_width > 0 && plane->limit_height > 0)
// 				printf("  Limits: width=%.2f, height=%.2f\n",
//					plane->limit_width, plane->limit_height);
// 		}
// 		else if (obj->type == CYLINDER)
// 		{
// 			cylinder = (t_cylinder *)obj->data;
// 			printf("Cylinder\n");
// 			printf("  Position: (%.2f, %.2f, %.2f)\n", cylinder->position.x,
// 				cylinder->position.y, cylinder->position.z);
// 			printf("  Orientation: (%.2f, %.2f, %.2f)\n",
// 				cylinder->orientation.x, cylinder->orientation.y,
// 				cylinder->orientation.z);
// 			printf("  Diameter: %.2f\n", cylinder->diameter);
// 			printf("  Height: %.2f\n", cylinder->height);
// 			printf("  Color: (%d, %d, %d, %d)\n", cylinder->color.r,
// 				cylinder->color.g, cylinder->color.b, cylinder->color.a);
// 		}
// 		else if (obj->type == CONE)
// 		{
// 			t_cone *cone = (t_cone *)obj->data;
// 			printf("Cone\n");
// 			printf("  Apex: (%.2f, %.2f, %.2f)\n", cone->apex.x,
// 				cone->apex.y, cone->apex.z);
// 			printf("  Direction: (%.2f, %.2f, %.2f)\n", cone->direction.x,
// 				cone->direction.y, cone->direction.z);
// 			printf("  Angle: %.2f\n", cone->angle);
// 			printf("  Height: %.2f\n", cone->height);
// 			printf("  Color: (%d, %d, %d, %d)\n", cone->color.r,
// 				cone->color.g, cone->color.b, cone->color.a);
// 		}
// 		else
// 		{
// 			printf("Unknown\n");
// 		}
// 		printf("\n");
// 	}
// }
// print_objects(&mini.scene);

// atexit(leaks);
int	main(int argc, char **argv)
{
	t_miniRT	mini;
	int			return_value;

	return_value = 0;
	if (argc != 2)
		exit_error("Usage: ./miniRT <filename>\n");
	return_value = init_mlx(&mini);
	if (return_value)
		return (return_value);
	init_scene(&mini.scene);
	return_value = parse_rt_file(argv[1], &mini.scene);
	convert_objects(&mini.scene);
	if (return_value)
		return (return_value);
	printf("----------------------\n");
	render_scene(mini.img, &mini);
	printf("----------------------\n");
	mlx_loop_hook(mini.mlx, loop, &mini);
	mlx_loop(mini.mlx);
	free_scene(&mini.scene);
	mlx_terminate(mini.mlx);
	return (return_value);
}
