/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_doors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 17:39:13 by frey-gal          #+#    #+#             */
/*   Updated: 2025/11/18 18:18:55 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static mlx_texture_t	*door_texture_frame(t_data *data, int frame)
{
	if (data->doors.open == true)
	{
		if (data->doors.frame_num >= 0 && data->doors.frame_num < 10)
			return (data->doors.texture[0]);
		if (data->doors.frame_num >= 10 && data->doors.frame_num < 20)
			return (data->doors.texture[1]);
		if (data->doors.frame_num >= 20 && data->doors.frame_num < 30)
			return (data->doors.texture[2]);
		if (data->doors.frame_num >= 30 && data->doors.frame_num < 40)
			return (data->doors.texture[3]);
		if (data->doors.frame_num >= 40 && data->doors.frame_num < 50)
			return (data->doors.texture[4]);
		if (data->doors.frame_num >= 50 && data->doors.frame_num < 60)
			return (data->doors.texture[5]);
		if (data->doors.frame_num >= 60)
		{
			data->doors.in_animation = false;
			return (data->doors.texture[6]);
		}
	}
	else
	{
		if (data->doors.frame_num >= 0 && data->doors.frame_num < 10)
			return (data->doors.texture[6]);
		if (data->doors.frame_num >= 10 && data->doors.frame_num < 20)
			return (data->doors.texture[5]);
		if (data->doors.frame_num >= 20 && data->doors.frame_num < 30)
			return (data->doors.texture[4]);
		if (data->doors.frame_num >= 30 && data->doors.frame_num < 40)
			return (data->doors.texture[3]);
		if (data->doors.frame_num >= 40 && data->doors.frame_num < 50)
			return (data->doors.texture[2]);
		if (data->doors.frame_num >= 50 && data->doors.frame_num < 60)
			return (data->doors.texture[1]);
		if (data->doors.frame_num >= 60)
		{
			data->doors.in_animation = false;
			return (data->doors.texture[0]);
		}
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
		{
			doors->open = true;
			doors->frame_num = 0;
			doors->in_animation = true;
		}
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

static void	get_door_x(t_data *data)
{
	t_doors	*d;

	d = &data->doors;
	if (data->raycast.side == 0)
		d->door_x = data->player.curr_y + d->ray_distance * data->raycast.ray_dir_y;
	else
		d->door_x = data->player.curr_x + d->ray_distance * data->raycast.ray_dir_x;
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
	if ((r->side == 0 && r->ray_dir_x <= 0) || \
		(r->ray_dir_y > 0 && r->side == 1))
		d->door_x = 1 - d->door_x;
	r->tex_x = (int)(d->door_x * (double)current_texture->width);
	r->tex_step = 1.0 * current_texture->width / d->line_height;
	r->tex_pos = (d->draw_start - WIN_HEIGHT / 2 + d->line_height / 2) * \
		r->tex_step;
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
