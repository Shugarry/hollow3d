/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:11:27 by frey-gal          #+#    #+#             */
/*   Updated: 2025/10/07 16:26:58 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	move_forth(t_data *data)
{
	double		new_x;
	double		new_y;
	int			length;

	new_x = data->player.curr_x + data->raycast.dir_x * MOVE_SPEED;
	new_y = data->player.curr_y + data->raycast.dir_y * MOVE_SPEED;
	length = ft_strlen(data->map[(int)new_y]);
	if (new_x >= 0 && (int)new_x < length)
		if (data->map[(int)data->player.curr_y][(int)new_x] != '1')
			data->player.curr_x = new_x;
	if (new_y >= 0 && (int)new_y < data->parsing.height)
		if (data->map[(int)new_y][(int)data->player.curr_x] != '1')
			data->player.curr_y = new_y;
}

static void	move_back(t_data *data)
{
	double		new_x;
	double		new_y;
	int			length;

	new_x = data->player.curr_x - data->raycast.dir_x * MOVE_SPEED;
	new_y = data->player.curr_y - data->raycast.dir_y * MOVE_SPEED;
	length = ft_strlen(data->map[(int)new_y]);
	if (new_x >= 0 && (int)new_x < length)
		if (data->map[(int)data->player.curr_y][(int)new_x] != '1')
			data->player.curr_x = new_x;
	if (new_y >= 0 && (int)new_y < data->parsing.height)
		if (data->map[(int)new_y][(int)data->player.curr_x] != '1')
			data->player.curr_y = new_y;
}

static void	move_left(t_data *data)
{
	double		new_x;
	double		new_y;
	int			length;

	new_x = data->player.curr_x - data->raycast.plane_x * MOVE_SPEED;
	new_y = data->player.curr_y - data->raycast.plane_y * MOVE_SPEED;
	length = ft_strlen(data->map[(int)new_y]);
	if (new_x >= 0 && (int)new_x < length)
		if (data->map[(int)data->player.curr_y][(int)new_x] != '1')
			data->player.curr_x = new_x;
	if (new_y >= 0 && (int)new_y < data->parsing.height)
		if (data->map[(int)new_y][(int)data->player.curr_x] != '1')
			data->player.curr_y = new_y;
}

static void	move_right(t_data *data)
{
	double		new_x;
	double		new_y;
	int			length;

	new_x = data->player.curr_x + data->raycast.plane_x * MOVE_SPEED;
	new_y = data->player.curr_y + data->raycast.plane_y * MOVE_SPEED;
	length = ft_strlen(data->map[(int)new_y]);
	if (new_x >= 0 && (int)new_x < length)
		if (data->map[(int)data->player.curr_y][(int)new_x] != '1')
			data->player.curr_x = new_x;
	if (new_y >= 0 && (int)new_y < data->parsing.height)
		if (data->map[(int)new_y][(int)data->player.curr_x] != '1')
			data->player.curr_y = new_y;
}

void	movement(t_data *data)
{
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
		move_forth(data);
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
		move_back(data);
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
		move_left(data);
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
		move_right(data);
}
