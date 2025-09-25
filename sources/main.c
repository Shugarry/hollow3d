/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 20:39:28 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/23 18:33:02 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"
#include <stdint.h>

void	main_hook(void *param) // loops this to detect key presses
{
	t_data	*data;

	data = param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
	{
		mlx_close_window(data->mlx);
		clean_exit(data, NULL, 0);
	}
	// need to go into function which redraws map
}

void	draw_floor(t_data *data)
{
	int	i;
	int	j;

	i = WIN_HEIGHT / 2;
	while (i < WIN_HEIGHT)
	{
		j = 0;
		while (j < WIN_WIDTH)
		{
			mlx_put_pixel(data->canvas, j, i, 0xFFFF0000);
			j++;
		}
		i++;
	}

}

void	draw_ceiling(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	while (i < WIN_HEIGHT / 2)
	{
		j = 0;
		while (j < WIN_WIDTH)
		{
			mlx_put_pixel(data->canvas, j, i, 0xFFF6FF00);
			j++;
		}
		i++;
	}
}

void	draw_canvas(t_data *data)
{
	data->canvas = mlx_new_image(data->mlx, 1280, 720);
	draw_ceiling(data);
	draw_floor(data);
	mlx_image_to_window(data->mlx, data->canvas, 0, 0);
}

void	provisional_map(t_data *data)
{
	char	*tmp;
	int		fd;
	int		i;

	i = 0;
	fd = open("maps/FELIX_TESTMAP", O_RDONLY);
	if (fd == -1)
		clean_exit(data, "open() failure at provisional_map()", 1);
	while ((tmp = get_next_line(fd)) != NULL)
	{
		data->map = realloc(data->map, sizeof(char *) * (i + 2));
		data->map[i] = tmp;
		i++;
	}
	data->map[i] = NULL;
	for (int i = 0; data->map[i]; i++)
		printf("map: %s", data->map[i]);
	for (int i = 0; data->map[i]; i++)
	{
		for (int j = 0; data->map[i][j]; j++)
		{
			if (data->map[i][j] == 'N')
			{
				data->player.start_x = j;
				data->player.start_y = i;
				data->player.curr_x = data->player.start_x;
				data->player.curr_y = data->player.start_y;
			}
		}
	}
	close(fd);
}

void	cast_rays(t_data *data)
{
	double	dir_x = -1, dir_y = 0; // PLAYER FACING POSITION
	double	plane_x = 0, plane_y = 0.66; // RAY ANGLES (FOV OF 66ishDEG)

	for (int x = 0; x < WIN_WIDTH; x++)
	{
		double camera_x = 2 * x / (double)WIN_WIDTH - 1;	// camera x is the xcoord on the camera plane that the current
															// xcoord on the screen represents so that left, middle and right
															// are -1, 0 and 1 respectively.

		double ray_dir_x = dir_x + plane_x * camera_x;	// direction vector for xcoords
		double ray_dir_y = dir_y + plane_y * camera_x;	// direction vector for ycoords

		int map_x = (int)data->player.curr_x;	// Current x and y positions of the ray on the map array 
		int map_y = (int)data->player.curr_y;

		double	side_dist_x;
		double	side_dist_y;

		double delta_dist_x = (ray_dir_x == 0) ? 1e30 : sqrt(1 + (ray_dir_y * ray_dir_y) / (ray_dir_x * ray_dir_x)); // pythagoras hypotenuse for the ray,
		double delta_dist_y = (ray_dir_y == 0) ? 1e30 : sqrt(1 + (ray_dir_x * ray_dir_x) / (ray_dir_y * ray_dir_y)); // this calculates the length

		double perp_wall_dist; // total length of the ray
		int step_x, step_y; // direction to step towards (+1 or -1 depending on cardinal direction)
		int hit = 0; // was a wall hit?
		int side;

		// need to calculate step x y and side dist x y
		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (data->player.curr_x - map_x) * delta_dist_x;
		}
		else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - data->player.curr_x) * delta_dist_x;
		}
		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (data->player.curr_y - map_y) * delta_dist_y;
		}
		else
		{
			step_y = -1;
			side_dist_y = (map_y + 1.0 - data->player.curr_y) * delta_dist_y;
		}
		while(hit == 0)
		{
			//jump to next map square, either in x-direction, or in y-direction
			if(side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 0;
			}
			else
		{
				side_dist_y += delta_dist_x;
				map_y += step_y;
				side = 1;
			}
			if(data->map[map_x][map_y] > 0)
				hit = 1;
		}
		if (side == 0)
			perp_wall_dist = (side_dist_x - delta_dist_x);
		else
			perp_wall_dist = (side_dist_y - delta_dist_y);
		
		int line_height = (int) (WIN_HEIGHT / perp_wall_dist);

		int	draw_start = -line_height / 2 + WIN_HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;

		int	draw_end = line_height / 2 + WIN_HEIGHT / 2;
		if (draw_end < 0)
			draw_end = 0;

		uint32_t color = (side == 1) ? 0xFFC2C2C2 : 0xFF696969;
		
		for (int l = draw_start; l < draw_end; l++)
			mlx_put_pixel(data->canvas, l, x, color);

	}
}

void	draw_walls(t_data *data)
{
	provisional_map(data);
	cast_rays(data);
}

void	start_mlx(t_data *data)
{
	data->mlx = mlx_init(1280, 720, "cub3d", false);
	if (!data->mlx)
		clean_exit(data, (char *)mlx_strerror(mlx_errno), EXIT_FAILURE);
	//data->resources = init_resources(data);
	draw_canvas(data);
	draw_walls(data);
	mlx_loop_hook(data->mlx, &main_hook, data);
	mlx_loop(data->mlx);
}

int	main(/* int ac, char **av */)
{
	t_data	data;

	ft_bzero(&data, sizeof(t_data));
	ft_bzero(&data.player, sizeof(t_player));
	ft_bzero(&data.textures, sizeof(t_textures));
	ft_bzero(&data.images, sizeof(t_images));
	//parsing
	//if (!data->)
	start_mlx(&data);
	clean_exit(&data, NULL, 0);
	return (0);
}
