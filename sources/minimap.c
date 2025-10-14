#include "cub3d.h"


void	draw_square(mlx_image_t *img, int start_x, int start_y, int size, uint32_t color)
{
	for (int y = 0; y < size; y++)
	{
		for (int x = 0; x < size; x++)
		{
			int px = start_x + x;
			int py = start_y + y;
			if (px >= 0 && px < (int)img->width && py >= 0 && py < (int)img->height)
				mlx_put_pixel(img, px, py, color);
		}
	}
}
void draw_minimap_layout(t_data *data)
{
    for (int y = 0; y < data->parsing.height; y++)
    {

        int row_len = ft_strlen(data->map[y]);

        for (int x = 0; x < row_len; x++)
        {
            char tile = data->map[y][x];
            uint32_t color;

            if (tile == '1')
                color = 0xFF0000FF;
            else if (tile == '0' || tile == 'N')
                color = 0x0000FFFF;
            //  else if (tile == 'N')
            //      color = 0x00FF00FF;
            else
                continue;

            int screen_x = x * MINI_TILE_SIZE;
            int screen_y = y * MINI_TILE_SIZE;

            draw_square(data->mini, screen_x, screen_y, MINI_TILE_SIZE, color);
        }
    }
}

void	draw_minimap_player(t_data *data)
{
	int px = (int)(data->player.x * MINI_TILE_SIZE);
	int py = (int)(data->player.y * MINI_TILE_SIZE);
	int radius = 2;

	for (int y = -radius; y <= radius; y++)
	{
		for (int x = -radius; x <= radius; x++)
		{
			if (x * x + y * y <= radius * radius)
			{
				int dx = px + x;
				int dy = py + y;
				if (dx >= 0 && dx < (int)data->parsing.width && dy >= 0 && dy < (int)data->parsing.height)
					mlx_put_pixel(data->mini, dx, dy, 0xFF0000FF);
			}
		}
	}
}
#define PLAYER_COLOR 0xFFFFFFFF
#define PLAYER_SIZE  4

void draw_player_on_minimap(t_data *data)
{
    double px = data->player.curr_x * MINI_TILE_SIZE;
    double py = data->player.curr_y * MINI_TILE_SIZE;


    int start_x = (int)(px - PLAYER_SIZE / 2);
    int start_y = (int)(py - PLAYER_SIZE / 2);

    draw_square(data->mini, start_x, start_y, PLAYER_SIZE, PLAYER_COLOR);
}


void	update_minimap(t_data *data)
{
	memset(data->mini->pixels, 0, data->mini->width * data->mini->height * sizeof(uint32_t));

	draw_minimap_layout(data);
	draw_minimap_player(data);
	draw_player_on_minimap(data);
}


void	init_mini(t_data *data)
{
	data->mini = mlx_new_image(data->mlx, data->parsing.width * MINI_SIZE, data->parsing.height * MINI_SIZE);
	mlx_image_to_window(data->mlx, data->mini, 0, 0);
	draw_square(data->mini, 0, 0, MINI_SIZE, 0xFF0000FF);
}
