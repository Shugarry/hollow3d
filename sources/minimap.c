/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 20:30:08 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/10 22:43:21 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_in_circle(int x, int y, int radius)
{
	int	dx;
	int	dy;

	dx = x - MINIMAP_CENTER_X;
	dy = y - MINIMAP_CENTER_Y;
	return (dx * dx + dy * dy <= radius * radius);
}

void	draw_line(mlx_image_t *img, t_data *data, int x1, int y1)
{
	int			i;
	uint32_t	*pixel;

	data->line.dx = x1 - MINIMAP_CENTER_X;
	data->line.dy = y1 - MINIMAP_CENTER_Y;
	data->line.steps = fmax(abs(data->line.dx), abs(data->line.dy));
	data->line.x_inc = data->line.dx / (float)data->line.steps;
	data->line.y_inc = data->line.dy / (float)data->line.steps;
	data->line.x = MINIMAP_CENTER_X;
	data->line.y = MINIMAP_CENTER_Y;
	i = 0;
	while (i <= data->line.steps)
	{
		if (data->line.x >= 0 && data->line.x < img->width && data->line.y \
				>= 0 && data->line.y < img->height)
		{
			pixel = (uint32_t *)(img->pixels + ((int)data->line.y * img->width \
						+ (int)data->line.x) * sizeof(uint32_t));
			*pixel = PLAYER_DIR_COLOR;
		}
		data->line.x += data->line.x_inc;
		data->line.y += data->line.y_inc;
		i++;
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

void	update_minimap(t_data *data)
{
	memset(data->mini->pixels, 0, data->mini->width * \
			data->mini->height * sizeof(uint32_t));
	data->circle.center_x = MINIMAP_CENTER_X;
	data->circle.center_y = MINIMAP_CENTER_Y;
	draw_minimap_circle(data->mini, data, MINI_RADIUS, 0x333333FF);
	draw_minimap_layout(data);
	draw_player_on_minimap(data);
	draw_minimap_circle_border(data);
	draw_enemies_on_minimap(data);
}

void	init_mini(t_data *data)
{
	int	size;

	size = MINIMAP_RADIUS * 2;
	data->mini = mlx_new_image(data->mlx, size, size);
	mlx_image_to_window(data->mlx, data->mini, 0, 0);
	data->circle.center_x = MINIMAP_CENTER_X;
	data->circle.center_y = MINIMAP_CENTER_Y;
	draw_minimap_circle(data->mini, data, MINIMAP_RADIUS, 0x333333FF);
}
