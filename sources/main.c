/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 20:39:28 by joshapir          #+#    #+#             */
/*   Updated: 2025/10/04 18:10:10 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

uint32_t	rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

bool	move_forth(t_data *data)
{
	double		new_x;
	double		new_y;
	int			length;

	new_x = data->player.curr_x + data->raycast.dir_x * MS;
	new_y = data->player.curr_y + data->raycast.dir_y * MS;
	length = ft_strlen(data->map[(int)new_y]);
	if (new_x >= 0 && (int)new_x < length)
		if (data->map[(int)data->player.curr_y][(int)new_x] != '1')
			data->player.curr_x = new_x;
	if (new_y >= 0 && (int)new_y < data->parsing.height)
		if (data->map[(int)new_y][(int)data->player.curr_x] != '1')
			data->player.curr_y = new_y;
	return (true);
}

bool	move_back(t_data *data)
{
	double		new_x;
	double		new_y;
	int			length;

	new_x = data->player.curr_x - data->raycast.dir_x * MS;
	new_y = data->player.curr_y - data->raycast.dir_y * MS;
	length = ft_strlen(data->map[(int)new_y]);
	if (new_x >= 0 && (int)new_x < length)
		if (data->map[(int)data->player.curr_y][(int)new_x] != '1')
			data->player.curr_x = new_x;
	if (new_y >= 0 && (int)new_y < data->parsing.height)
		if (data->map[(int)new_y][(int)data->player.curr_x] != '1')
			data->player.curr_y = new_y;
	return (true);
}

bool	move_left(t_data *data)
{
	double		new_x;
	double		new_y;
	int			length;

	new_x = data->player.curr_x - data->raycast.plane_x * MS;
	new_y = data->player.curr_y - data->raycast.plane_y * MS;
	length = ft_strlen(data->map[(int)new_y]);
	if (new_x >= 0 && (int)new_x < length)
		if (data->map[(int)data->player.curr_y][(int)new_x] != '1')
			data->player.curr_x = new_x;
	if (new_y >= 0 && (int)new_y < data->parsing.height)
		if (data->map[(int)new_y][(int)data->player.curr_x] != '1')
			data->player.curr_y = new_y;
	return (true);
}

bool	move_right(t_data *data)
{
	double		new_x;
	double		new_y;
	int			length;

	new_x = data->player.curr_x + data->raycast.plane_x * MS;
	new_y = data->player.curr_y + data->raycast.plane_y * MS;
	length = ft_strlen(data->map[(int)new_y]);
	if (new_x >= 0 && (int)new_x < length)
		if (data->map[(int)data->player.curr_y][(int)new_x] != '1')
			data->player.curr_x = new_x;
	if (new_y >= 0 && (int)new_y < data->parsing.height)
		if (data->map[(int)new_y][(int)data->player.curr_x] != '1')
			data->player.curr_y = new_y;
	return (true);
}

void	movement(t_data *data)
{
	bool	keypress;

	keypress = false;
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		move_forth(data);
		keypress = true;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		move_back(data);
		keypress = true;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		move_left(data);
		keypress = true;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		move_right(data);
		keypress = true;
	}
	if (keypress)
		raycaster(data, false);
}

void	main_hook(void *param)
{
	t_data		*data;
	t_raycast	*rc;

	data = param;
	rc = &data->raycast;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
	{
		mlx_close_window(data->mlx);
		clean_exit(data, NULL, 0);
	}
	movement(data);
}

void draw_3d_ceiling(t_data *data)
{
    int x;
	int y;
	int tex_x;
	int tex_y;
	int colour;
	int *pixels;
	int tex_width;
	int tex_height;
	//TODO put vars into struct
	x = 0;
	y = 0;
    mlx_image_t *img   = data->images.ceiling;
    pixels = (int *)data->textures.ceiling->pixels;
    tex_width = data->textures.ceiling->width;
    tex_height = data->textures.ceiling->height;

    while (y < WIN_HEIGHT / 2)
    {
		x = 0;
        while (x < WIN_WIDTH)
        {
            tex_x = (x * tex_width) / WIN_WIDTH;
            tex_y = (y * tex_height) / (WIN_HEIGHT / 2);

            colour = pixels[tex_y * tex_width + tex_x];
            mlx_put_pixel(img, x, y, colour);
			x++;
        }
		y++;
    }
    mlx_image_to_window(data->mlx, img, 0, 0);
}

void draw_3d_floor(t_data *data)
{
    int x, y, tex_x, tex_y, colour;
    int *pixels;
    int tex_width, tex_height;

    mlx_image_t *img = data->images.floor;
    pixels = (int *)data->textures.floor->pixels;
    tex_width = data->textures.floor->width;
    tex_height = data->textures.floor->height;

    y = 0;
    while (y < WIN_HEIGHT / 2)
    {
        x = 0;
        while (x < WIN_WIDTH)
        {
            tex_x = (x * tex_width) / WIN_WIDTH;
            tex_y = (y * tex_height) / (WIN_HEIGHT / 2);

            colour = pixels[tex_y * tex_width + tex_x];
            mlx_put_pixel(img, x, y, colour);
            x++;
        }
        y++;
    }
    mlx_image_to_window(data->mlx, img, 0, WIN_HEIGHT / 2);
}

void load_textures(t_data *data)
{
    data->textures.ceiling = mlx_load_png("resources/sky_texture.png");
    data->textures.floor   = mlx_load_png("resources/stone_2.png");
    if (!data->textures.ceiling || !data->textures.floor)
        clean_exit(data, "Error loading textures\n", 1);

    data->images.ceiling = mlx_new_image(data->mlx, WIN_WIDTH, WIN_HEIGHT / 2);
    data->images.floor   = mlx_new_image(data->mlx, WIN_WIDTH, WIN_HEIGHT / 2);

    if (!data->images.ceiling || !data->images.floor)
        clean_exit(data, "Error creating texture images\n", 1);

    draw_3d_ceiling(data);
    draw_3d_floor(data);
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
	if (rcast->ray_dir_x == 0)
		rcast->delta_dist_x = INFINITY;
	else
		rcast->delta_dist_x = fabs(1 / rcast->ray_dir_x);
	if (rcast->ray_dir_y == 0)
		rcast->delta_dist_y = INFINITY;
	else
		rcast->delta_dist_y = fabs(1 / rcast->ray_dir_y);
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
		color = rgba(128, 128, 128, 128);
	else
		color = rgba(128, 128, 128, 255);
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
	for (int l = 0; l <= rcast->draw_start; l++)
		mlx_put_pixel(data->canvas, x, l, rgba(0, 0, 0, 255));
	for (int l = rcast->draw_start; l <= rcast->draw_end; l++)
		mlx_put_pixel(data->canvas, x, l, get_color(rcast->side));
	for (int l = rcast->draw_end; l <= WIN_HEIGHT; l++)
		mlx_put_pixel(data->canvas, x, l, rgba(0, 0, 0, 255));
}

void	raycaster(t_data *data, bool first_call)
{
	if (first_call)
		starting_vars(data);
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		cast_rays(data, x);
		step_in_dir(data);
		ray_find_wall(data);
		draw_walls(data, x);
		// printf("rcast->perp_wall_dist %f\n", rcast->perp_wall_dist);
		// printf("rcast->line_height %i\n", rcast->line_height);
		// printf("rcast->draw_start %i, rcast->draw_end %i\n", rcast->draw_start, rcast->draw_end);
		// printf("map_x %d, map_y %d\n", rcast->map_x, rcast->map_y);
		// printf("player_x %f, player_y %f\n", data->player.curr_x, data->player.curr_y);
		// printf("\n");
	}
}

void	start_mlx(t_data *data)
{
	data->mlx = mlx_init(1280, 720, "cub3d", false);
	if (!data->mlx)
		clean_exit(data, (char *)mlx_strerror(mlx_errno), EXIT_FAILURE);
	data->canvas = mlx_new_image(data->mlx, 1280, 720);
	raycaster(data, true);
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
	ft_bzero(&data->raycast, sizeof(t_paths));
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
	data.map = data.parsing.map;
	data.player.curr_x = data.player.y + 0.5;
	data.player.curr_y = data.player.x + 0.5;
	start_mlx(&data);
	clean_exit(&data, NULL, 0);
	return (0);
}
