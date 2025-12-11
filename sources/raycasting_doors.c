/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_doors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:39:13 by frey-gal          #+#    #+#             */
/*   Updated: 2025/12/11 18:53:34 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	check_door(t_data *data)
{
	t_raycast	*r;
	double		intersection;

	r = &data->raycast;
	if (r->side == 0)
	{
		intersection = r->side_dist_x - 0.5;
		if (intersection < r->side_dist_y)
		{
			data->doors.ray_distance = intersection;
			data->doors.door_found = true;
			return ;
		}
	}
	else
	{
		intersection = r->side_dist_y - 0.5;
		if (intersection < r->side_dist_x)
		{
			data->doors.ray_distance = intersection;
			data->doors.door_found = true;
			return ;
		}
	}
	data->doors.door_found = false;
}

static mlx_texture_t	*door_texture_frame(t_data *data, int frame)
{
	mlx_texture_t	*ret;

	ret = NULL;
	if (data->doors.open == true)
	{
		ret = draw_frames_opening(data, frame);
		if (ret != NULL)
			return (ret);
	}
	else
	{
		ret = draw_frames_closing(data, frame);
		if (ret != NULL)
			return (ret);
	}
	return (data->doors.texture[0]);
}

static mlx_texture_t	*get_door_texture(t_data *data, t_doors *doors)
{
	mlx_texture_t	*texture;

	if (doors->in_animation == true)
		return (door_texture_frame(data, doors->frame_num));
	if (!doors->open)
	{
		if (doors->ray_distance <= 5)
			get_door_texture_helper(doors);
		else
			return (doors->texture[0]);
	}
	else
	{
		if (doors->ray_distance > 5)
		{
			doors->open = false;
			doors->frame_num = 0;
			doors->in_animation = true;
		}
		else
			return (doors->texture[6]);
	}
	return (doors->texture[0]);
}

static void	texturize_walls(t_data *data, int x)
{
	t_doors			*d;
	t_raycast		*r;
	int				line;
	uint32_t		color;
	mlx_texture_t	*current_texture;

	d = &data->doors;
	r = &data->raycast;
	get_door_x(data);
	current_texture = get_door_texture(data, &data->doors);
	d->door_x -= floor(d->door_x);
	line = d->draw_start;
	while (line < d->draw_end)
	{
		r->tex_y = (int)r->tex_pos & (current_texture->height - 1);
		r->tex_pos += r->tex_step;
		color = get_tex_pixel(current_texture, r->tex_x, r->tex_y, false);
		if (color)
			mlx_put_pixel(data->canvas, x, line, color);
		line++;
	}
}

void	draw_door(t_data *data, int x)
{
	t_doors	*d;

	d = &data->doors;
	d->line_height = (int)(WIN_HEIGHT / d->ray_distance);
	d->draw_start = -d->line_height / 2 + WIN_HEIGHT / 2;
	if (d->draw_start < 0)
		d->draw_start = 0;
	if (d->draw_start >= WIN_HEIGHT)
		d->draw_start = WIN_HEIGHT - 1;
	d->draw_end = d->line_height / 2 + WIN_HEIGHT / 2;
	if (d->draw_end < 0)
		d->draw_end = 0;
	if (d->draw_end >= WIN_HEIGHT)
		d->draw_end = WIN_HEIGHT - 1;
	texturize_walls(data, x);
}
