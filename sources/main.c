/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 20:39:28 by joshapir          #+#    #+#             */
/*   Updated: 2025/11/10 20:13:41 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	main_hook(void *param)
{
	t_data		*data;
	double		start;
	double		elapsed;

	data = param;
	start = get_time_seconds();
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
	{
		mlx_close_window(data->mlx);
		clean_exit(data, NULL, 0);
	}
	movement(data);
	camera(data);

	raycaster(data);
	update_enemies(data);
	sort_enemies(data);
	int i = 0;
	while (i < data->enemy_count)
    {
        if (data->enemies[i].alive)
            draw_enemy(data, &data->enemies[i]);
        i++;
    }
	elapsed = get_time_seconds() - start;
	if (elapsed < SIXTY_FPS)
		usleep((useconds_t)((SIXTY_FPS - elapsed) * 1e6));
	update_minimap(data);
	fps_counter(data);
	sword_animation(data);
	data->animation.frame_num += 1;
}

void	start_mlx(t_data *data)
{
	data->mlx = mlx_init(WIN_WIDTH, WIN_HEIGHT, "cub3d", false);
	if (!data->mlx)
		clean_exit(data, (char *)mlx_strerror(mlx_errno), EXIT_FAILURE);
	data->canvas = mlx_new_image(data->mlx, WIN_WIDTH, WIN_HEIGHT);
	data->wall_distances = malloc(sizeof(double) * WIN_WIDTH);
	if (!data->wall_distances)
		exit(1);
	starting_vars(data);
	data->raycast.old_time = get_time_seconds();
	data->raycast.time = data->raycast.old_time;
	mlx_image_to_window(data->mlx, data->canvas, 0, 0);
	mlx_loop_hook(data->mlx, &main_hook, data);
	init_enemies(data);
	init_mini(data);
	mlx_loop(data->mlx);
}

void	init_structs(t_data *data)
{
	int	i;

	i = 0;
	ft_bzero(data, sizeof(t_data));
	ft_bzero(&data->raycast, sizeof(t_paths));
	ft_bzero(&data->animation, sizeof(t_paths));
	ft_bzero(&data->player, sizeof(t_player));
	ft_bzero(&data->textures, sizeof(t_textures));
	ft_bzero(&data->parsing, sizeof(t_parsing));
	ft_bzero(&data->parsing.paths, sizeof(t_paths));
	ft_bzero(&data->enemy_vars, sizeof(t_enemy_vars));
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
	data->textures.floor = mlx_load_png("resources/floor.png");
	data->textures.ceiling = mlx_load_png("resources/ceiling.png");
	if (!data->textures.north || !data->textures.south
		|| !data->textures.east || !data->textures.west)
		clean_exit(data, "Could not get textures", 1);
}

int	main(int argc, char **argv)
{
	t_data	data;

	init_structs(&data);
	data.animation.sword[0] = mlx_load_png("resources/sword_animation/frame_0.png");
	data.animation.sword[1] = mlx_load_png("resources/sword_animation/frame_1.png");
	data.animation.sword[2] = mlx_load_png("resources/sword_animation/frame_2.png");
	data.animation.sword[3] = mlx_load_png("resources/sword_animation/frame_3.png");
	data.animation.sword[4] = mlx_load_png("resources/sword_animation/frame_4.png");
	data.textures.fps_ui = mlx_load_png("resources/fps_ui.png");
	parsing(&data, argv, argc);
	get_parsed_variables(&data);
	start_mlx(&data);
	clean_exit(&data, NULL, 0);
	return (0);
}
