/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 23:08:26 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/11 21:12:37 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	calculate_enemy_distance(t_data *data, int i, t_enemy_vars *vars)
{
	double	dir_x;
	double	dir_y;

	vars->dx = data->enemies[i].x - data->player.curr_x;
	vars->dy = data->enemies[i].y - data->player.curr_y;
	data->enemies[i].distance = sqrt(vars->dx * vars->dx + vars->dy * vars->dy);
	dir_x = -vars->dx / data->enemies[i].distance;
	dir_y = -vars->dy / data->enemies[i].distance;
	vars->new_x = data->enemies[i].x + dir_x * vars->move_speed;
	vars->new_y = data->enemies[i].y + dir_y * vars->move_speed;
}

void	check_if_can_move(t_data *data, int i, t_enemy_vars *vars)
{
	int		j;
	double	dx;
	double	dy;

	j = 0;
	vars->can_move = 1;
	if (data->map[(int)vars->new_y][(int)vars->new_x] == '1')
		vars->can_move = 0;
	while (j < data->enemy_count && vars->can_move)
	{
		if (j != i && data->enemies[j].alive)
		{
			dx = vars->new_x - data->enemies[j].x;
			dy = vars->new_y - data->enemies[j].y;
			if (sqrt(dx * dx + dy * dy) < 1.0)
				vars->can_move = 0;
		}
		j++;
	}
}

void	enemy_loop(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->enemy_count)
	{
		if (data->enemies[i].alive)
			draw_enemy(data, &data->enemies[i]);
		i++;
	}
}
