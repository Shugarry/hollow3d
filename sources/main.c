/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 20:39:28 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/29 19:06:16 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"
#include <stdint.h>

uint32_t rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void	main_hook(void *param) // loops this to detect key presses
{
	t_data		*data;
	t_raycast	info;

	data = param;
	info = data->raycast;
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

void	loop_vars(t_data *data, t_raycast *info, int x)
{
	info->camera_x = 2 * x / (double)WIN_WIDTH - 1;	// camera x is the xcoord on the camera plane that the current
	// xcoord on the screen represents so that left, middle and right
	// are -1, 0 and 1 respectively.

	info->ray_dir_x = info->dir_x + info->plane_x * info->camera_x;	// direction vector for xcoords
	info->ray_dir_y = info->dir_y + info->plane_y * info->camera_x;	// direction vector for ycoords

	info->map_x = (int)data->player.curr_x;	// Current x and y positions of the ray on the map array
	info->map_y = (int)data->player.curr_y;

	info->delta_dist_x = (info->ray_dir_x == 0) ? 1e30 : fabs(1 / info->ray_dir_x); // pythagoras hypotenuse for the ray,
	info->delta_dist_y = (info->ray_dir_y == 0) ? 1e30 : fabs(1 / info->ray_dir_y); // this calculates the length

	//info->perp_wall_dist; total length of the ray
	//int info->step_x, info->step_y; // direction to step towards (+1 or -1 depending on cardinal direction)
	info->hit = false; // was a wall info->hit?
}

void	choose_step_dir(t_data *data, t_raycast *info)
{
	// need to calculate step x y and info.side dist x y
	if (info->ray_dir_x < 0)
	{
		info->step_x = -1;
		info->side_dist_x = (data->player.curr_x - info->map_x) * info->delta_dist_x;
	}
	else
	{
		info->step_x = 1;
		info->side_dist_x = (info->map_x + 1.0 - data->player.curr_x) * info->delta_dist_x;
	}
	if (info->ray_dir_y < 0)
	{
		info->step_y = -1;
		info->side_dist_y = (data->player.curr_y - info->map_y) * info->delta_dist_y;
	}
	else
	{
		info->step_y = 1;
		info->side_dist_y = (info->map_y + 1.0 - data->player.curr_y) * info->delta_dist_y;
	}
}

void	find_wall(t_data *data, t_raycast *info)
{
	while(info->hit == false)
	{
		//jump to next map square, either in x-direction, or in y-direction
		if(info->side_dist_x < info->side_dist_y)
		{
			info->side_dist_x += info->delta_dist_x;
			info->map_x += info->step_x;
			info->side = 0;
		}
		else
		{
			info->side_dist_y += info->delta_dist_y;
			info->map_y += info->step_y;
			info->side = 1;
		}
		if(data->map[info->map_x][info->map_y] == '1')
			info->hit = true;
	}
}

void	cast_rays(t_data *data)
{
	t_raycast	info;

	info = data->raycast;
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		loop_vars(data, &info, x);
		choose_step_dir(data, &info);
		find_wall(data, &info);
		if (info.side == 0)
			info.perp_wall_dist = (info.side_dist_x - info.delta_dist_x);
		else
			info.perp_wall_dist = (info.side_dist_y - info.delta_dist_y);
		info.line_height = (int)(WIN_HEIGHT / info.perp_wall_dist);
		info.draw_start = -info.line_height / 2 + WIN_HEIGHT / 2;
		if (info.draw_start < 0)
			info.draw_start = 0;
		info.draw_end = info.line_height / 2 + WIN_HEIGHT / 2;
		if (info.draw_end < 0)
			info.draw_end = 0;
		uint32_t color = (info.side == 1) ? rgba(255, 0, 0, 255) : rgba(0, 0, 255, 255);
		for (int l = info.draw_start; l <= info.draw_end; l++)
			mlx_put_pixel(data->canvas, x, l, color);
	}
}

void	raycast_start_vars(t_data *data)
{
	data->raycast.dir_x = -1;
	data->raycast.dir_y = 0;
	data->raycast.plane_x = 0; // RAY ANGLES (FOV OF 66ishDEG)
	data->raycast.plane_y = 0.66;
}

void	draw_walls(t_data *data)
{
	raycast_start_vars(data);
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

#include "cub3d.h"
#include "libraries/libft/libft.h"
#include <stdlib.h>

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

void	calculate_height(t_map *map, int start)
{
	int		i;
	char	**grid;

	grid = map->grid;
	i = 0;
	start = 0;
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
	calculate_height(map, i);
	find_player(&j, map);
	if (!map->player_found)
		error_and_free("Player not found", map);
	if (!flood_fill(map, map->player->x, map->player->y))
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
	t_map	*map;
	int		fd;

	(void) map;
	fd = open(argv[1], O_RDONLY);
	if (fd == -1 || argc != 2)
		error_and_free("ERROR", NULL);
	init_map_vars(&map);
	init_map(argv[1], &map);
	map->map = ft_strdup_double(map->grid);
	map->elements_grid = ft_strdup_double(map->grid);
	check_map(map);
	data->parsing = *map;
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
	start_mlx(&data);
	clean_exit(&data, NULL, 0);
	return (0);
}
