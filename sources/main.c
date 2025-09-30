/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 20:39:28 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/30 17:31:50 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

uint32_t rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void	main_hook(void *param) // loops this to detect key presses
{
	t_data		*data;

	data = param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
	{
		mlx_close_window(data->mlx);
		clean_exit(data, NULL, 0);
	}
	// if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	// {
    //   if(data->map[int(info.pos_x + info.dir_x * MS)][int(info.pos_y)] == false) info.pos_x += info.dir_x * MS;
    //   if(data->map[int(info.pos_x)][int(info.pos_y + info.dir_y * MS)] == false) info.pos_y += info.dir_y * MS;
	// }
	// if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	// {

	// }
	// if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	// {

	// }
	// if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	// {
	// 	if(worldMap[int(info.pos_x - info.dir_x * MS)][int(info.pos_y)] == false) info.pos_x -= info.dir_x * MS;
	// 	if(worldMap[int(info.pos_x)][int(info.pos_y - info.dir_y * MS)] == false) info.pos_y -= info.dir_y * MS;
	// }
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
			mlx_put_pixel(data->canvas, j, i, rgba(0, 0, 0, 255));
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
			mlx_put_pixel(data->canvas, j, i, rgba(0, 0, 0, 255));
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
}

// void	provisional_map(t_data *data)
// {
// 	char	*tmp;
// 	int		fd;
// 	int		i;
//
// 	i = 0;
// 	fd = open("maps/FELIX_TESTMAP", O_RDONLY);
// 	if (fd == -1)
// 		clean_exit(data, "open() failure at provisional_map()", 1);
// 	while ((tmp = get_next_line(fd)) != NULL)
// 	{
// 		data->map = realloc(data->map, sizeof(char *) * (i + 2));
// 		data->map[i] = tmp;
// 		i++;
// 	}
// 	data->map[i] = NULL;
// 	for (int i = 0; data->map[i]; i++)
// 		printf("map: %s", data->map[i]);
// 	for (int i = 0; data->map[i]; i++)
// 	{
// 		for (int j = 0; data->map[i][j]; j++)
// 		{
// 			if (data->map[i][j] == 'N')
// 			{
// 				data->player.x = j;
// 				data->player.y = i;
// 				data->player.curr_x = data->player.x + 0.5;
// 				data->player.curr_y = data->player.y + 0.5;
// 			}
// 		}
// 	}
// 	close(fd);
// }

void	cast_rays(t_data *data)
{
	double	dir_x = -1, dir_y = 0; // PLAYER FACING POSITION
	double	plane_x = 0, plane_y = -0.66; // RAY ANGLES (FOV OF 66ishDEG)

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

		double delta_dist_x = (ray_dir_x == 0) ? INFINITY : fabs(1 / ray_dir_x); // pythagoras hypotenuse for the ray,
		double delta_dist_y = (ray_dir_y == 0) ? INFINITY : fabs(1 / ray_dir_y); // this calculates the length

		double perp_wall_dist; // total length of the ray
		int step_x, step_y; // direction to step towards (+1 or -1 depending on cardinal direction)
		bool hit = false; // was a wall hit?
		int side;

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
			step_y = 1;
			side_dist_y = (map_y + 1.0 - data->player.curr_y) * delta_dist_y;
		}
		while(hit == false)
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
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 1;
			}
			if(data->map[map_y][map_x] == '1')
				hit = true;
		}
		if (side == 0)
			perp_wall_dist = (side_dist_x - delta_dist_x);
		else
			perp_wall_dist = (side_dist_y - delta_dist_y);
		
		int line_height = (int)(WIN_HEIGHT / perp_wall_dist);

		int	draw_start = -line_height / 2 + WIN_HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;//WIN_HEIGHT - 1; // might be 0
		int	draw_end = line_height / 2 + WIN_HEIGHT / 2;
		if (draw_end < 0)
			draw_end = 0;
		if (draw_end >= WIN_HEIGHT) draw_end = WIN_HEIGHT - 1;

		uint32_t color = (side == 1) ? rgba(255, 0, 0, 255) : rgba(0, 0, 255, 255);
		
		printf("perp_wall_dist %f\n", perp_wall_dist);
		printf("line_height %i\n", line_height);
		printf("draw_start %i, draw_end %i\n", draw_start, draw_end);
		printf("map_x %d, map_y %d\n", map_x, map_y);
		printf("\n");
		for (int l = draw_start; l <= draw_end; l++)
			mlx_put_pixel(data->canvas, x, l, color);
	}
}

void	draw_walls(t_data *data)
{
	//provisional_map(data);
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
	mlx_image_to_window(data->mlx, data->canvas, 0, 0);
	mlx_loop_hook(data->mlx, &main_hook, data);
	mlx_loop(data->mlx);
}

int	is_map_line(char *line)
{
	int	i;

	i = 0;
	while (line && line[i] == ' ')
		i++;
	if (line[i] == '\0')
		return (0);
	while (line[i])
	{
		if (line[i] != '0' && line[i] != '1' && \
				line[i] != 'N' && line[i] != 'S' && \
				line[i] != 'E' && line[i] != 'W' && \
				line[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

void	calculate_height(t_map *map)
{
	int		i;
	char	**grid;

	grid = map->grid;
	i = 0;
	while (grid[i] && is_map_line(grid[i]))
		i++;
	while (grid[i])
	{
		if (is_map_line(grid[i]))
		{
			error_and_free("Map must not be seperated", map);
		}
		i++;
	}
	map->height = i;
}

int	check_map(t_map *map)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	find_elements(map);
	remove_elements(&map->grid, i);
	remove_elements(&map->map, 0);
	calculate_height(map);
	find_player(&j, map);
	if (!map->player_found)
		error_and_free("Player not found", map);
	if (!flood_fill(map, map->player.x, map->player.y))
		error_and_free("Map_invalid", map);
	return (1);
}

void	print_grid(char **grid)
{
	int	i;

	i = 0;
	if (!grid)
		return ;
	while (grid[i])
	{
		printf("%s", grid[i]);
		i++;
	}
}

void	parsing(t_data *data, char **argv, int argc)
{
	int		fd;

	//(void) map;
	fd = open(argv[1], O_RDONLY);
	if (fd == -1 || argc != 2)
		error_and_free("ERROR", NULL);
	init_map_vars(&data->parsing);
	init_map(argv[1], &data->parsing);
	data->parsing.map = ft_strdup_double(data->parsing.grid);
	data->parsing.elements_grid = ft_strdup_double(data->parsing.grid);
	check_map(&data->parsing);
	//cleanup(&map);
}

int	main(int argc, char **argv)
{
	t_data	data;

	ft_bzero(&data, sizeof(t_data));
	ft_bzero(&data.player, sizeof(t_player));
	ft_bzero(&data.textures, sizeof(t_textures));
	ft_bzero(&data.images, sizeof(t_images)); // NOTE: Make init function
	parsing(&data, argv, argc);
	data.map = data.parsing.map;
	for (int i = 0; data.map[i]; i++)
	{
		printf("%s\n", data.map[i]);
	}
	//start_mlx(&data);
	//clean_exit(&data, NULL, 0);
	return (0);
}
