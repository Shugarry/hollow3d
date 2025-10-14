/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 20:39:28 by joshapir          #+#    #+#             */
/*   Updated: 2025/10/14 17:19:30 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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
	update_minimap(data);
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
	init_mini(data);
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
	ft_bzero(&data->parsing, sizeof(t_parsing));
	ft_bzero(&data->parsing.paths, sizeof(t_paths));
	while (i < 3)
	{
		data->parsing.paths.c_color[i] = -1;
		data->parsing.paths.f_color[i] = -1;
		i++;
	}
}

void	get_parsed_variables(t_data *data)
{
	int	*tmp;

	data->map = data->parsing.map;
	data->player.curr_x = data->player.y + 0.5;
	data->player.curr_y = data->player.x + 0.5;
	tmp = data->parsing.paths.c_color;
	data->textures.ceiling_color = rgba(tmp[0], tmp[1], tmp[2], 255);
	tmp = data->parsing.paths.f_color;
	data->textures.floor_color = rgba(tmp[0], tmp[1], tmp[2], 255);
	data->textures.north = mlx_load_png(data->parsing.paths.n_tex);
	data->textures.east = mlx_load_png(data->parsing.paths.e_tex);
	data->textures.south = mlx_load_png(data->parsing.paths.s_tex);
	data->textures.west = mlx_load_png(data->parsing.paths.w_tex);
	if (!data->textures.north || !data->textures.south
		|| !data->textures.east || !data->textures.west)
		clean_exit(data, "Could not get textures", 1);
}

int	main(int argc, char **argv)
{
	t_data	data;

	init_structs(&data);
	parsing(&data, argv, argc);
	get_parsed_variables(&data);
	start_mlx(&data);
	clean_exit(&data, NULL, 0);
	return (0);
}
