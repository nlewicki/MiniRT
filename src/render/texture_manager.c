/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nlewicki <nlewicki@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:56:00 by nlewicki          #+#    #+#             */
/*   Updated: 2025/05/12 13:05:29 by nlewicki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

t_color sample_texture(t_texture *tex, double u, double v)
{
	t_color color = {255, 255, 255, 255};  // Default white color
	uint32_t *pixels;
	int x, y;

	if (!tex || !tex->enabled || !tex->texture)
		return color;

	pixels = (uint32_t *)tex->texture->pixels;
	x = (int)(u * (tex->texture->width - 1));
	y = (int)(v * (tex->texture->height - 1));

	// Clamp coordinates
	x = x < 0 ? 0 : x;
	x = x >= (int)tex->texture->width ? tex->texture->width - 1 : x;
	y = y < 0 ? 0 : y;
	y = y >= (int)tex->texture->height ? tex->texture->height - 1 : y;

	uint32_t pixel = pixels[y * tex->texture->width + x];

	// Extract RGBA components
	color.r = (pixel >> 24) & 0xFF;
	color.g = (pixel >> 16) & 0xFF;
	color.b = (pixel >> 8) & 0xFF;
	color.a = pixel & 0xFF;

	return color;
}

void init_texture(t_texture *tex)
{
	if (!tex)
		return;
	tex->xpm = NULL;
	tex->texture = NULL;
	tex->path = NULL;
	tex->enabled = false;
}

bool load_texture(t_texture *tex, const char *path, mlx_t *mlx)
{
	if (!tex || !path || !mlx)
		return false;

	printf("Loading texture: %s\n", path);
	tex->path = ft_strdup(path);
	tex->xpm = mlx_load_xpm42(path);
	if (!tex->xpm)
	{
		printf("Failed to load texture: %s\n", path);
		free(tex->path);
		tex->path = NULL;
		return false;
	}
	printf("Successfully loaded texture: %s\n", path);

	tex->texture = &tex->xpm->texture;
	tex->enabled = true;
	return true;
}

void cleanup_texture(t_texture *tex)
{
	if (!tex)
		return;

	if (tex->xpm)
		mlx_delete_xpm42(tex->xpm);
	if (tex->path)
		free(tex->path);

	tex->xpm = NULL;
	tex->texture = NULL;
	tex->path = NULL;
	tex->enabled = false;
}

t_color get_texture_color(t_texture *tex, t_vec3 point, t_object *obj)
{
	double u, v;

	if (!tex || !tex->enabled || !tex->texture)
	{
		printf("Texture not applied: enabled=%d texture=%p\n",
			tex ? tex->enabled : -1,
			tex ? (void*)tex->texture : NULL);
		return obj->color;
	}

	switch (obj->type)
	{
		case SPHERE:
		{
			t_sphere *sphere = (t_sphere *)obj->data;
			t_vec3 local = vec_normalize(vec_sub(point, sphere->center));
			u = 0.5 + atan2(local.z, local.x) / (2 * M_PI);
			v = 0.5 - asin(local.y) / M_PI;
			break;
		}
		case PLANE:
		{
			t_plane *plane = (t_plane *)obj->data;
			t_vec3 normal = plane->orientation;
			t_vec3 u_axis, v_axis;

			if (fabs(normal.x) < fabs(normal.y) && fabs(normal.x) < fabs(normal.z))
				u_axis = (t_vec3){0, -normal.z, normal.y};
			else if (fabs(normal.y) < fabs(normal.z))
				u_axis = (t_vec3){-normal.z, 0, normal.x};
			else
				u_axis = (t_vec3){-normal.y, normal.x, 0};

			u_axis = vec_normalize(u_axis);
			v_axis = vec_cross(normal, u_axis);

			t_vec3 relative_pos = vec_sub(point, plane->position);
			u = vec_dot(relative_pos, u_axis);
			v = vec_dot(relative_pos, v_axis);

			if (plane->limit_width > 0 && plane->limit_height > 0)
			{
				u = u / plane->limit_width + 0.5;
				v = v / plane->limit_height + 0.5;
			}
			else
			{
				u = fmod(u, 1.0);
				v = fmod(v, 1.0);
				if (u < 0) u += 1.0;
				if (v < 0) v += 1.0;
			}
			break;
		}
		case CYLINDER:
		{
			t_cylinder *cyl = (t_cylinder *)obj->data;
			t_vec3 axis = vec_normalize(cyl->orientation);
			t_vec3 cp = vec_sub(point, cyl->position);
			double height = vec_dot(cp, axis);
			t_vec3 radial = vec_normalize(vec_sub(cp, vec_mul(axis, height)));

			double theta = atan2(radial.z, radial.x);
			if (theta < 0)
				theta += 2 * M_PI;

			u = theta / (2 * M_PI);
			v = height / cyl->height;
			break;
		}
		case CONE:
		{
			t_cone *cone = (t_cone *)obj->data;
			t_vec3 ap = vec_sub(point, cone->apex);
			double height = vec_dot(ap, cone->direction);
			t_vec3 radial = vec_normalize(vec_sub(ap, vec_mul(cone->direction, height)));

			double theta = atan2(radial.z, radial.x);
			if (theta < 0)
				theta += 2 * M_PI;

			u = theta / (2 * M_PI);
			v = height / cone->height;
			break;
		}
		default:
			return obj->color;
	}

	return sample_texture(tex, u, v);
}
