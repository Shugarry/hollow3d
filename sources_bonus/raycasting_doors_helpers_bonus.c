/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_doors_helpers.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 18:43:13 by frey-gal          #+#    #+#             */
/*   Updated: 2025/12/11 21:06:24 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

mlx_texture_t	*draw_frames_opening(t_data *data, int frame)
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
	return (NULL);
}

mlx_texture_t	*draw_frames_closing(t_data *data, int frame)
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
	return (NULL);
}

void	get_door_texture_helper(t_doors *doors)
{
	doors->open = true;
	doors->frame_num = 0;
	doors->in_animation = true;
}

void	get_door_x(t_data *data)
{
	t_doors	*d;

	d = &data->doors;
	if (data->raycast.side == 0)
		d->door_x = data->player.curr_y + d->ray_distance * \
			data->raycast.ray_dir_y;
	else
		d->door_x = data->player.curr_x + d->ray_distance * \
			data->raycast.ray_dir_x;
}

void	texturize_doors_helper(t_data *data, t_raycast *r, t_doors *d)
{
	d->door_x -= floor(d->door_x);
	if ((r->side == 0 && r->ray_dir_x <= 0) || \
		(r->ray_dir_y > 0 && r->side == 1))
		d->door_x = 1 - d->door_x;
}
