/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 20:30:08 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/01 18:19:23 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


#define PLAYER_COLOR 0xFFFFFFFF
#define PLAYER_DIR_COLOR 0xFF00FFFF
#define PLAYER_SIZE 4
#define MINIMAP_RADIUS 80
#define MINIMAP_VIEW_RADIUS 8
#define MINIMAP_CENTER_X (MINIMAP_RADIUS)
#define MINIMAP_CENTER_Y (MINIMAP_RADIUS)


int is_in_circle(int x, int y, int center_x, int center_y, int radius)
{
	int dx = x - center_x;
	int dy = y - center_y;
	return (dx * dx + dy * dy <= radius * radius);
}

void draw_line(mlx_image_t *img, int x0, int y0, int x1, int y1, uint32_t color)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int steps = fmax(abs(dx), abs(dy));
	float x_inc = dx / (float)steps;
	float y_inc = dy / (float)steps;
	float x = x0;
	float y = y0;
	int i;
	uint32_t *pixel;

	i = 0;
	while (i <= steps)
	{
		if (x >= 0 && x < img->width && y >= 0 && y < img->height)
		{
			pixel = (uint32_t *)(img->pixels + ((int)y * img->width + (int)x) * sizeof(uint32_t));
			*pixel = color;
		}
		x += x_inc;
		y += y_inc;
		i++;
	}
}

void draw_square(mlx_image_t *img, int start_x, int start_y, int size, uint32_t color)
{
	int x;
	int y;
	int px;
	int py;

	y = 0;
	while (y < size)
	{
		x = 0;
		while (x < size)
		{
			px = start_x + x;
			py = start_y + y;
			if (px >= 0 && px < (int)img->width && py >= 0 && py < (int)img->height)
				mlx_put_pixel(img, px, py, color);
			x++;
		}
		y++;
	}
}

void draw_minimap_circle(mlx_image_t *img, int center_x, int center_y, int radius, uint32_t color)
{
	int y;
	int x;
	int px;
	int py;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x*x + y*y <= radius*radius)
			{
				px = center_x + x;
				py = center_y + y;
				if (px >= 0 && px < (int)img->width && py >= 0 && py < (int)img->height)
					mlx_put_pixel(img, px, py, color);
			}
			x++;
		}
		y++;
	}
}

void draw_tile_on_minimap(t_data *data, int x, int y, uint32_t colour)
{
	int tile_x, tile_y, px, py;
	int screen_x = MINIMAP_CENTER_X + (x - data->player.curr_x) * MINI_TILE_SIZE;
	int screen_y = MINIMAP_CENTER_Y + (y - data->player.curr_y) * MINI_TILE_SIZE;

	tile_y = 0;
	while (tile_y < MINI_TILE_SIZE)
	{
		tile_x = 0;
		while (tile_x < MINI_TILE_SIZE)
		{
			px = screen_x + tile_x;
			py = screen_y + tile_y;
			if (px >= 0 && px < (int)data->mini->width &&
					py >= 0 && py < (int)data->mini->height &&
					is_in_circle(px, py, MINIMAP_CENTER_X, MINIMAP_CENTER_Y, MINIMAP_RADIUS))
				mlx_put_pixel(data->mini, px, py, colour);
			tile_x++;
		}
		tile_y++;
	}
}

void draw_minimap_layout(t_data *data)
{
	int x, y;
	char tile;
	uint32_t colour;
	int player_tile_x = (int)(data->player.curr_x);
	int player_tile_y = (int)(data->player.curr_y);

	y = player_tile_y - MINIMAP_VIEW_RADIUS;
	while (y <= player_tile_y + MINIMAP_VIEW_RADIUS)
	{
		if (y >= 0 && y < data->parsing.height)
		{
			x = player_tile_x - MINIMAP_VIEW_RADIUS;
			while (x <= player_tile_x + MINIMAP_VIEW_RADIUS)
			{
				if (x >= 0 && x < (int)ft_strlen(data->map[y]))
				{
					tile = data->map[y][x];
					if (tile == '1')
						colour = 0xFF0000FF;
					else if (tile == '0' || tile == 'N' || tile == 'S' || tile == 'E' || tile == 'W')
						colour = 0x0000FFFF;
					else
						colour = 0;
					if (colour)
						draw_tile_on_minimap(data, x, y, colour);
				}
				x++;
			}
		}
		y++;
	}
}

void draw_minimap_player(t_data *data)
{
	int px = (int)(data->player.x * MINI_TILE_SIZE);
	int py = (int)(data->player.y * MINI_TILE_SIZE);
	int radius = 2;
	int y;
	int x;

	y = -radius;
	while (y <= radius)
	{
		x = -radius;
		while (x <= radius)
		{
			if (x * x + y * y <= radius * radius)
			{
				int dx = px + x;
				int dy = py + y;
				if (dx >= 0 && dx < (int)data->parsing.width && dy >= 0 && dy < (int)data->parsing.height)
					mlx_put_pixel(data->mini, dx, dy, 0xFF0000FF);
			}
			x++;
		}
		y++;
	}
}

void draw_player_on_minimap(t_data *data)
{

	int px = MINIMAP_CENTER_X;
	int py = MINIMAP_CENTER_Y;
	int line_len = 30;
	int end_x = px + data->raycast.dir_x * line_len;
	int end_y = py + data->raycast.dir_y * line_len;

	draw_square(data->mini, px - PLAYER_SIZE / 2, py - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_COLOR);
	draw_line(data->mini, px, py, end_x, end_y, PLAYER_DIR_COLOR);
}

void draw_minimap_circle_border(t_data *data)
{
	int center_x = MINIMAP_CENTER_X;
	int center_y = MINIMAP_CENTER_Y;
	int radius = MINIMAP_RADIUS;
	int angle = 0;
	while (angle < 360)
	{
		int x = center_x + radius * cos(angle * M_PI / 180.0);
		int y = center_y + radius * sin(angle * M_PI / 180.0);
		if (x >= 0 && x < (int)data->mini->width && y >= 0 && y < (int)data->mini->height)
			mlx_put_pixel(data->mini, x, y, 0xFFFFFFFF);
		angle++;
	}
}

void draw_minimap_background(t_data *data)
{
	int y;
	int x;

	y = 0;
	while (y < (int)data->mini->height)
	{
		x = 0;
		while (x < (int)data->mini->width)
		{
			if (is_in_circle(x, y, MINIMAP_CENTER_X, MINIMAP_CENTER_Y, MINIMAP_RADIUS))
			{
				mlx_put_pixel(data->mini, x, y, 0x33333340);
			}
			x++;
		}
		y++;
	}
}

void draw_enemies_on_minimap(t_data *data)
{
    int	i;
    int	enemy_screen_x;
    int	enemy_screen_y;

    i = 0;
    while (i < data->enemy_count)
    {
        if (data->enemies[i].alive)
        {
            enemy_screen_x = MINIMAP_CENTER_X + (data->enemies[i].x - data->player.curr_x) * MINI_TILE_SIZE;
            enemy_screen_y = MINIMAP_CENTER_Y + (data->enemies[i].y - data->player.curr_y) * MINI_TILE_SIZE;
			if (is_in_circle(enemy_screen_x, enemy_screen_y, MINIMAP_CENTER_X, MINIMAP_CENTER_Y, MINIMAP_RADIUS))
            	draw_minimap_circle(data->mini, enemy_screen_x, enemy_screen_y, 3, 0xFF0000FF);
        }
        i++;
    }
}

void update_minimap(t_data *data)
{
	memset(data->mini->pixels, 0, data->mini->width * data->mini->height * sizeof(uint32_t));
	draw_minimap_circle(data->mini, MINIMAP_CENTER_X, MINIMAP_CENTER_Y, MINIMAP_RADIUS, 0x333333FF);
	draw_minimap_layout(data);
	draw_player_on_minimap(data);
	draw_minimap_circle_border(data);
	draw_enemies_on_minimap(data);
}

void init_mini(t_data *data)
{

	int size = MINIMAP_RADIUS * 2;
	data->mini = mlx_new_image(data->mlx, size, size);
	mlx_image_to_window(data->mlx, data->mini, 0, 0);
	draw_minimap_circle(data->mini, MINIMAP_CENTER_X, MINIMAP_CENTER_Y, MINIMAP_RADIUS, 0x333333FF);
}
