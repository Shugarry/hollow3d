#include "cub3d.h"


#define PLAYER_COLOR 0xFFFFFFFF
#define PLAYER_DIR_COLOR 0xFF00FFFF
#define PLAYER_SIZE 4

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
           pixel  = (uint32_t *)(img->pixels + ((int)y * img->width + (int)x) * sizeof(uint32_t));
            *pixel = color;
        }
        x += x_inc;
        y += y_inc;
		i++;
    }
}

void	draw_square(mlx_image_t *img, int start_x, int start_y, int size, uint32_t color)
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
void draw_minimap_layout(t_data *data)
{
	int x;
	int y;
	int row_len;
	char tile;
	uint32_t colour;

  	y = 0;
	colour = 0;
	while (y < data->parsing.height)
	{
    	row_len = ft_strlen(data->map[y]);
    	x = 0;
    	while (x < row_len)
    	{
        	tile = data->map[y][x];
        	if (tile == '1')
            	colour = 0xFF0000FF;
        else if (tile == '0' || tile == 'N' || tile == 'S' || tile == 'E' || tile == 'W')
            colour = 0x0000FFFF;
        // else if (tile == 'N')
        //     color = 0x00FF00FF;
        else
        {
            x++;
            continue;
        }

        int screen_x = x * MINI_TILE_SIZE;
        int screen_y = y * MINI_TILE_SIZE;

        draw_square(data->mini, screen_x, screen_y, MINI_TILE_SIZE, colour);

        x++;
    }
    y++;
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


void draw_player_on_minimap(t_data *data)
{
    double px = data->player.curr_x * MINI_TILE_SIZE;
    double py = data->player.curr_y * MINI_TILE_SIZE;
	int line_len;
	int end_x;
	int end_y;

    draw_square(data->mini, px - PLAYER_SIZE / 2, py - PLAYER_SIZE / 2, PLAYER_SIZE, PLAYER_COLOR);

    line_len = 30;
    end_x = px + data->raycast.dir_x * line_len;
    end_y = py + data->raycast.dir_y * line_len;
    draw_line(data->mini, px, py, end_x, end_y, PLAYER_DIR_COLOR);
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
