/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 20:39:28 by joshapir          #+#    #+#             */
/*   Updated: 2025/10/07 16:49:48 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

uint32_t	rgba(int r, int g, int b, int a)
{
    return (r << 24 | g << 16 | b << 8 | a);
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

void	main_hook(void *param)
{
	t_data		*data;

	data = param;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
	{
		mlx_close_window(data->mlx);
		clean_exit(data, NULL, 0);
	}
	movement(data);
	camera(data);
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

void	start_mlx(t_data *data)
{
	data->mlx = mlx_init(1280, 720, "cub3d", false);
	if (!data->mlx)
		clean_exit(data, (char *)mlx_strerror(mlx_errno), EXIT_FAILURE);
	data->canvas = mlx_new_image(data->mlx, 1280, 720);
	starting_vars(data);
	raycaster(data);
	mlx_image_to_window(data->mlx, data->canvas, 0, 0);
	mlx_loop_hook(data->mlx, &main_hook, data);
	mlx_loop(data->mlx);
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
