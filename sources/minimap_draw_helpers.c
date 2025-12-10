/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_draw_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 23:08:38 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/10 23:09:31 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	layout_loop(int colour, int x, int y, t_data *data)
{
	char	tile;

	while (x <= data->mini_m.player_tile_x + MINIMAP_VIEW_RADIUS)
	{
		if (x >= 0 && x < (int)ft_strlen(data->map[y]))
		{
			tile = data->map[y][x];
			if (tile == '1')
				colour = 0xFF0000FF;
			else if (tile == '0' || tile == 'N' || tile == 'S' \
					|| tile == 'E' || tile == 'W')
				colour = 0x0000FFFF;
			else
				colour = 0;
			if (colour)
				draw_tile_on_minimap(data, x, y, colour);
		}
		x++;
	}
}

void	draw_minimap_layout(t_data *data)
{
	int			x;
	int			y;
	char		tile;
	uint32_t	colour;

	data->mini_m.player_tile_x = (int)(data->player.curr_x);
	data->mini_m.player_tile_y = (int)(data->player.curr_y);
	y = data->mini_m.player_tile_y - MINIMAP_VIEW_RADIUS;
	while (y <= data->mini_m.player_tile_y + MINIMAP_VIEW_RADIUS)
	{
		if (y >= 0 && y < data->parsing.height)
		{
			x = data->mini_m.player_tile_x - MINIMAP_VIEW_RADIUS;
			layout_loop(colour, x, y, data);
		}
		y++;
	}
}

void	draw_tile_on_minimap(t_data *data, int x, int y, uint32_t colour)
{
	int	px;
	int	py;

	data->mini_m.screen_x = MINIMAP_CENTER_X + (x - \
			data->player.curr_x) * MINI_TILE_SIZE;
	data->mini_m.screen_y = MINIMAP_CENTER_Y + (y - \
			data->player.curr_y) * MINI_TILE_SIZE;
	data->mini_m.tile_y = 0;
	while (data->mini_m.tile_y < MINI_TILE_SIZE)
	{
		data->mini_m.tile_x = 0;
		while (data->mini_m.tile_x < MINI_TILE_SIZE)
		{
			px = data->mini_m.screen_x + data->mini_m.tile_x;
			py = data->mini_m.screen_y + data->mini_m.tile_y;
			if (px >= 0 && px < (int)data->mini->width && \
					py >= 0 && py < (int)data->mini->height && \
					is_in_circle(px, py, MINIMAP_RADIUS))
				mlx_put_pixel(data->mini, px, py, colour);
			data->mini_m.tile_x++;
		}
		data->mini_m.tile_y++;
	}
}

void	draw_minimap_circle(mlx_image_t *img, t_data *data, \
		int radius, uint32_t color)
{
	int	y;
	int	x;
	int	px;
	int	py;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x * x + y * y <= radius * radius)
			{
				px = data->circle.center_x + x;
				py = data->circle.center_y + y;
				if (px >= 0 && px < (int)img->width && py >= 0 && py \
						< (int)img->height)
					mlx_put_pixel(img, px, py, color);
			}
			x++;
		}
		y++;
	}
}

void	draw_square(mlx_image_t *img, int start_x, int start_y, int size)
{
	int	x;
	int	y;
	int	px;
	int	py;

	y = 0;
	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			px = start_x + x;
			py = start_y + y;
			if (px >= 0 && px < (int)img->width && py >= 0 && py \
					< (int)img->height)
				mlx_put_pixel(img, px, py, PLAYER_COLOR);
			x++;
		}
		y++;
	}
}
