/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_draw_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 23:09:47 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/10 23:10:14 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_enemies_on_minimap(t_data *data)
{
	int	i;
	int	enemy_screen_x;
	int	enemy_screen_y;

	i = 0;
	while (i < data->enemy_count)
	{
		if (data->enemies[i].alive)
		{
			enemy_screen_x = MINIMAP_CENTER_X + (data->enemies[i].x \
					- data->player.curr_x) * MINI_TILE_SIZE;
			enemy_screen_y = MINIMAP_CENTER_Y + (data->enemies[i].y \
					- data->player.curr_y) * MINI_TILE_SIZE;
			data->circle.center_x = enemy_screen_x;
			data->circle.center_y = enemy_screen_y;
			if (is_in_circle(enemy_screen_x, enemy_screen_y, MINIMAP_RADIUS))
				draw_minimap_circle(data->mini, data, 3, 0xFF0000FF);
		}
		i++;
	}
}

void	draw_minimap_background(t_data *data)
{
	int	y;
	int	x;

	y = 0;
	while (y < (int)data->mini->height)
	{
		x = 0;
		while (x < (int)data->mini->width)
		{
			if (is_in_circle(x, y, MINIMAP_RADIUS))
			{
				mlx_put_pixel(data->mini, x, y, 0x33333340);
			}
			x++;
		}
		y++;
	}
}

void	draw_minimap_circle_border(t_data *data)
{
	int	x;
	int	y;

	data->mini_m.angle = 0;
	while (data->mini_m.angle < 360)
	{
		x = MINIMAP_CENTER_X + MINIMAP_RADIUS * \
			cos(data->mini_m.angle * M_PI / 180.0);
		y = MINIMAP_CENTER_Y + MINIMAP_RADIUS * \
			sin(data->mini_m.angle * M_PI / 180.0);
		if (x >= 0 && x < (int)data->mini->width && \
				y >= 0 && y < (int)data->mini->height)
			mlx_put_pixel(data->mini, x, y, 0xFFFFFFFF);
		data->mini_m.angle++;
	}
}

void	draw_player_on_minimap(t_data *data)
{
	int	line_len;
	int	end_x;
	int	end_y;

	line_len = 30;
	end_x = MINIMAP_CENTER_X + data->raycast.dir_x * line_len;
	end_y = MINIMAP_CENTER_Y + data->raycast.dir_y * line_len;
	draw_square(data->mini, MINIMAP_CENTER_X - PLAYER_SIZE / 2, \
			MINIMAP_CENTER_Y - PLAYER_SIZE / 2, PLAYER_SIZE);
	draw_line(data->mini, data, end_x, end_y);
}

void	draw_minimap_player(t_data *data)
{
	int	x;
	int	y;

	data->mini_m.radius = 2;
	y = -data->mini_m.radius;
	while (y <= data->mini_m.radius)
	{
		x = -data->mini_m.radius;
		while (x <= data->mini_m.radius)
		{
			if (x * x + y * y <= data->mini_m.radius * data->mini_m.radius)
			{
				data->mini_m.dir_x = (int)(data->player.x * MINI_TILE_SIZE) + x;
				data->mini_m.dir_y = (int)(data->player.y * MINI_TILE_SIZE) + y;
				if (data->mini_m.dir_x >= 0 && data->mini_m.dir_x < \
						(int)data->parsing.width && data->mini_m.dir_y \
						>= 0 && data->mini_m.dir_y < (int)data->parsing.height)
					mlx_put_pixel(data->mini, data->mini_m.dir_x, \
							data->mini_m.dir_y, 0xFF0000FF);
			}
			x++;
		}
		y++;
	}
}
