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

uint32_t	rgba(int r, int g, int b, int a)
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
 //      if(data->map[int(info.pos_x + info.dir_x * MS)][int(info.pos_y)] == false) info.pos_x += info.dir_x * MS;
 //      if(data->map[int(info.pos_x)][int(info.pos_y + info.dir_y * MS)] == false) info.pos_y += info.dir_y * MS;
	// }
	// if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	// {
	//
	// }
	// if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	// {
	//
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

void	make_canvas(t_data *data)
{
	data->canvas = mlx_new_image(data->mlx, 1280, 720);
	draw_ceiling(data);
	draw_floor(data);
}

void	starting_vars(t_data *data)
{
	t_raycast	*rcast;

	rcast = &data->raycast;
	rcast->dir_x = -1;
	rcast->dir_y = -0; // Direction of vectors on x y axis
	rcast->plane_x = 0;
	rcast->plane_y = -0.66; // RAY ANGLES (FOV OF 66ishDEG)
}

void	cast_rays(t_data *data, int x)
{
	t_raycast	*rcast;

	rcast = &data->raycast;
	rcast->camera_x = 2 * x / (double)WIN_WIDTH - 1;	// camera x is the xcoord on the camera plane that the current
														// xcoord on the screen represents so that left, middle and right
														// are -1, 0 and 1 respectively.
	rcast->ray_dir_x = rcast->dir_x + rcast->plane_x * rcast->camera_x; // direction vector for xcoords
	rcast->ray_dir_y = rcast->dir_y + rcast->plane_y * rcast->camera_x; // direction vector for ycoords
	rcast->map_x = (int)data->player.curr_x;	// Current x and y positions of the ray on the map array 
	rcast->map_y = (int)data->player.curr_y;
	rcast->delta_dist_x = (rcast->ray_dir_x == 0) ? INFINITY : fabs(1 / rcast->ray_dir_x); // pythagoras hypotenuse for the ray,
	rcast->delta_dist_y = (rcast->ray_dir_y == 0) ? INFINITY : fabs(1 / rcast->ray_dir_y); // this calculates the length

	rcast->hit = false; // was a wall hit?
}

void	step_in_dir(t_data *data)
{
	t_raycast	*rcast;

	rcast = &data->raycast;
	if (rcast->ray_dir_x < 0)
	{
		rcast->step_x = -1;
		rcast->side_dist_x = (data->player.curr_x - rcast->map_x) * rcast->delta_dist_x;
	}
	else
	{
		rcast->step_x = 1;
		rcast->side_dist_x = (rcast->map_x + 1.0 - data->player.curr_x) * rcast->delta_dist_x;
	}
	if (rcast->ray_dir_y < 0)
	{
		rcast->step_y = -1;
		rcast->side_dist_y = (data->player.curr_y - rcast->map_y) * rcast->delta_dist_y;
	}
	else
	{
		rcast->step_y = 1;
		rcast->side_dist_y = (rcast->map_y + 1.0 - data->player.curr_y) * rcast->delta_dist_y;
	}
}

void	ray_find_wall(t_data *data)
{
	t_raycast *rcast;

	rcast = &data->raycast;
	while(rcast->hit == false)
	{
		//jump to next map square, either in x-direction, or in y-direction
		if(rcast->side_dist_x < rcast->side_dist_y)
		{
			rcast->side_dist_x += rcast->delta_dist_x;
			rcast->map_x += rcast->step_x;
			rcast->side = 0;
		}
		else
		{
			rcast->side_dist_y += rcast->delta_dist_y;
			rcast->map_y += rcast->step_y;
			rcast->side = 1;
		}
		if(data->map[rcast->map_y][rcast->map_x] == '1')
			rcast->hit = true;
	}
	if (rcast->side == 0)
		rcast->perp_wall_dist = (rcast->side_dist_x - rcast->delta_dist_x);
	else
		rcast->perp_wall_dist = (rcast->side_dist_y - rcast->delta_dist_y);
}

uint32_t	get_color(int side)
{
	uint32_t color;
	
	if (side == 1)
		color = rgba(255, 0, 0, 255);
	else
		color = rgba(0, 0, 255, 255);
	return (color);
}

void	draw_walls(t_data *data, int x)
{
	t_raycast *rcast;

	rcast = &data->raycast;
	rcast->line_height = (int)(WIN_HEIGHT / rcast->perp_wall_dist);
	rcast->draw_start = -rcast->line_height / 2 + WIN_HEIGHT / 2;
	if (rcast->draw_start < 0)
		rcast->draw_start = 0;
	if (rcast->draw_start >= WIN_HEIGHT)
		rcast->draw_start = WIN_HEIGHT - 1;
	rcast->draw_end = rcast->line_height / 2 + WIN_HEIGHT / 2;
	if (rcast->draw_end < 0)
		rcast->draw_end = 0;
	if (rcast->draw_end >= WIN_HEIGHT)
		rcast->draw_end = WIN_HEIGHT - 1;
	for (int l = rcast->draw_start; l <= rcast->draw_end; l++)
		mlx_put_pixel(data->canvas, x, l, get_color(rcast->side));
}

void	raycaster(t_data *data)
{
	t_raycast	*rcast;

	rcast = &data->raycast;
	starting_vars(data);
	// implement movement through function parameters
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		cast_rays(data, x);
		step_in_dir(data);
		ray_find_wall(data);
		draw_walls(data, x);
		printf("rcast->perp_wall_dist %f\n", rcast->perp_wall_dist);
		printf("rcast->line_height %i\n", rcast->line_height);
		printf("rcast->draw_start %i, rcast->draw_end %i\n", rcast->draw_start, rcast->draw_end);
		printf("map_x %d, map_y %d\n", rcast->map_x, rcast->map_y);
		printf("player_x %f, player_y %f\n", data->player.curr_x, data->player.curr_y);
		printf("\n");
	}
}

void	start_mlx(t_data *data)
{
	data->mlx = mlx_init(1280, 720, "cub3d", false);
	if (!data->mlx)
		clean_exit(data, (char *)mlx_strerror(mlx_errno), EXIT_FAILURE);
	//data->resources = init_resources(data); NOTE: do later
	make_canvas(data);
	raycaster(data);
	mlx_image_to_window(data->mlx, data->canvas, 0, 0);
	mlx_loop_hook(data->mlx, &main_hook, data);
	mlx_loop(data->mlx);
}

bool	is_map_line(char *line)
{
	int	i;

	i = 0;
	while (line && line[i] == ' ')
		i++;
	if (line[i] == '\0')
		return (false);
	while (line[i])
	{
		if (line[i] != '0' && line[i] != '1' && \
				line[i] != 'N' && line[i] != 'S' && \
				line[i] != 'E' && line[i] != 'W' && \
				line[i] != ' ')
			return (false);
		i++;
	}
	return (true);
}

void	calculate_height(t_data *data)
{
	int		i;
	char	**grid;

	grid = data->parsing.grid;
	i = 0;
	while (grid[i] && is_map_line(grid[i]))
		i++;
	while (grid[i])
	{
		if (is_map_line(grid[i]))
			clean_exit(data, "Map must not be seperated", 1);
		i++;
	}
	data->parsing.height = i;
}

int	check_map(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	find_elements(data);
	remove_elements(&data->parsing.grid, i);
	data->parsing.map = ft_strdup_double(data, data->parsing.grid);
	calculate_height(data);
	find_player(data, &j);
	if (!data->parsing.player_found)
		clean_exit(data, "Error: Player not found", 1);
	if (!flood_fill(data, data->player.x, data->player.y))
		clean_exit(data, "Error: Invalid map", 1);
	return (1);
}

void	parsing(t_data *data, char **argv, int argc)
{
	int		fd;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1 || argc != 2)
		clean_exit(data, "Incorrect argument count", 1);
	init_map(data, argv[1]);
	data->parsing.map = ft_strdup_double(data, data->parsing.grid);
	data->parsing.elements_grid = ft_strdup_double(data, data->parsing.grid);
	check_map(data);
}

void	init_structs(t_data *data)
{
	int	i;

	i = 0;
	ft_bzero(data, sizeof(t_data));
	ft_bzero(&data->player, sizeof(t_player));
	ft_bzero(&data->textures, sizeof(t_textures));
	ft_bzero(&data->images, sizeof(t_images));
	ft_bzero(&data->parsing, sizeof(t_parsing));
	ft_bzero(&data->parsing.paths, sizeof(t_paths));
	while (i < 3)
	{
		data->parsing.paths.c_colour[i] = -1;
		data->parsing.paths.f_colour[i] = -1;
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_data	data;

	init_structs(&data);
	parsing(&data, argv, argc);
	check_parsed_values(&data);
	data.map = data.parsing.map;
	data.player.curr_x = data.player.y + 0.5;
	data.player.curr_y = data.player.x + 0.5;
	start_mlx(&data);
	clean_exit(&data, NULL, 0);
	return (0);
}
