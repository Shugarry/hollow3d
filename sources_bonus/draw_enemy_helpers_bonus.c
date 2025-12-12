/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_enemy_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 23:08:10 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/11 20:00:40 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	update_enemy_pos(t_data *data, int i, t_enemy_vars *vars)
{
	if (data->enemies[i].distance > 2.0)
	{
		data->enemies[i].in_range == 0;
		check_if_can_move(data, i, vars);
		if (vars->can_move)
		{
			data->enemies[i].x = vars->new_x;
			data->enemies[i].y = vars->new_y;
		}
	}
	else if (data->enemies[i].distance < 2.0)
	{
		data->enemies[i].in_range = 1;
		if (data->enemies[i].distance < 0.5)
		{
			data->player.dead = 1;
			clean_exit(data, "player died\n", 1);
		}
	}
}

void	update_enemies(t_data *data)
{
	int				i;
	t_enemy_vars	*vars;

	vars = &data->enemy_vars;
	i = 0;
	while (i < data->enemy_count)
	{
		if (data->enemies[i].alive)
		{
			calculate_enemy_distance(data, i, vars);
			vars->move_speed = 0.05;
			update_enemy_pos(data, i, vars);
			if (data->enemies[i].distance > 2.0)
				data->enemies[i].in_range = 0;
		}
		i++;
	}
}

void	sort_enemies(t_data *data)
{
	int		i;
	int		j;
	t_enemy	temp;

	i = 0;
	while (i < data->enemy_count - 1)
	{
		j = 0;
		while (j < data->enemy_count - i - 1)
		{
			if (data->enemies[j].distance < data->enemies[j + 1].distance)
			{
				temp = data->enemies[j];
				data->enemies[j] = data->enemies[j + 1];
				data->enemies[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	get_sprite_transform(t_data *data, t_enemy *enemy, double *transform_x, \
double *transform_y)
{
	double	sprite_x;
	double	sprite_y;
	double	inv_det;

	sprite_x = enemy->x - data->player.curr_x;
	sprite_y = enemy->y - data->player.curr_y;
	inv_det = 1.0 / (data->raycast.plane_x * data->raycast.dir_y \
			- data->raycast.dir_x * data->raycast.plane_y);
	*transform_x = inv_det * (data->raycast.dir_y * sprite_x - \
			data->raycast.dir_x * sprite_y);
	*transform_y = inv_det * (-data->raycast.plane_y * sprite_x + \
			data->raycast.plane_x * sprite_y);
}

void	get_sprite_dimensions(double transform_x, \
		double transform_y, int *screen_x, int *height)
{
	*screen_x = (int)((WIN_WIDTH / 2) * (1 + transform_x / transform_y));
	*height = abs((int)(WIN_HEIGHT / transform_y));
}
