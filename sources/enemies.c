/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 14:05:31 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/10 22:52:48 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	count_enemies(t_data *data)
{
	int	count;
	int	y;
	int	x;

	count = 0;
	y = 0;
	while (y < data->parsing.height)
	{
		x = 0;
		while (data->map[y][x])
		{
			if (data->map[y][x] == 'e')
				count++;
			x++;
		}
		y++;
	}
	return (count);
}

void	place_enemies(t_data *data)
{
	int	y;
	int	x;
	int	enemy_index;

	enemy_index = 0;
	y = 0;
	while (y < data->parsing.height)
	{
		x = 0;
		while (data->map[y][x])
		{
			if (data->map[y][x] == 'e')
			{
				data->enemies[enemy_index].x = x + 0.5;
				data->enemies[enemy_index].y = y + 0.5;
				data->enemies[enemy_index].alive = true;
				enemy_index++;
			}
			x++;
		}
		y++;
	}
}

void	init_enemies(t_data *data)
{
	int				i;
	mlx_texture_t	*enemy_tex;

	i = 0;
	data->enemy_count = count_enemies(data);
	if (data->enemy_count == 0)
	{
		data->enemies = NULL;
		return ;
	}
	data->enemies = malloc(sizeof(t_enemy) * data->enemy_count);
	if (!data->enemies)
		exit(1);
	place_enemies(data);
	enemy_tex = mlx_load_png("resources/skeleton_w.png");
	if (!enemy_tex)
		clean_exit(data, "Could not lead enemy texture", 1);
	while (i < data->enemy_count)
		data->enemies[i++].texture = enemy_tex;
}

void	check_distance_loop(t_data *data, int i, t_enemy_vars *vars)
{
	while (i < data->enemy_count)
	{
		if (data->enemies[i].alive && data->enemies[i].in_range)
		{
			vars->dx = data->enemies[i].x - data->player.curr_x;
			vars->dy = data->enemies[i].y - data->player.curr_y;
			vars->res = vars->dx * data->raycast.dir_x + \
			vars->dy * data->raycast.dir_y;
			if (vars->res > 0 && data->enemies[i].distance < \
					vars->closest_distance)
			{
				vars->closest_distance = data->enemies[i].distance;
				vars->closest_enemy = i;
			}
		}
		i++;
	}
}

void	check_if_dead(t_data *data)
{
	int				i;
	t_enemy_vars	vars;

	vars.closest_enemy = -1;
	vars.closest_distance = 999999.0;
	i = 0;
	check_distance_loop(data, i, &vars);
	if (vars.closest_enemy != -1 && !data->s_animation && data->sword_hit == 0)
	{
		data->enemies[vars.closest_enemy].alive = false;
		data->s_animation = 1;
		data->sword_hit = 1;
	}
}
