/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:13:48 by frey-gal          #+#    #+#             */
/*   Updated: 2025/10/07 16:27:07 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	pan_camera_left(t_data *data)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = data->raycast.dir_x;
	old_plane_x = data->raycast.plane_x;
	data->raycast.dir_x = data->raycast.dir_x * \
		cos(-ROTATE_SPEED) - data->raycast.dir_y * sin(-ROTATE_SPEED);
	data->raycast.dir_y = old_dir_x * \
		sin(-ROTATE_SPEED) + data->raycast.dir_y * cos(-ROTATE_SPEED);
	data->raycast.plane_x = data->raycast.plane_x * \
		cos(-ROTATE_SPEED) - data->raycast.plane_y * sin(-ROTATE_SPEED);
	data->raycast.plane_y = old_plane_x * \
		sin(-ROTATE_SPEED) + data->raycast.plane_y * cos(-ROTATE_SPEED);
}

static void	pan_camera_right(t_data *data)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = data->raycast.dir_x;
	old_plane_x = data->raycast.plane_x;
	data->raycast.dir_x = data->raycast.dir_x * \
		cos(ROTATE_SPEED) - data->raycast.dir_y * sin(ROTATE_SPEED);
	data->raycast.dir_y = old_dir_x * \
		sin(ROTATE_SPEED) + data->raycast.dir_y * cos(ROTATE_SPEED);
	data->raycast.plane_x = data->raycast.plane_x * \
		cos(ROTATE_SPEED) - data->raycast.plane_y * sin(ROTATE_SPEED);
	data->raycast.plane_y = old_plane_x * \
		sin(ROTATE_SPEED) + data->raycast.plane_y * cos(ROTATE_SPEED);
}

void	camera(t_data *data)
{
	bool	keypress;

	keypress = false;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
	{
		pan_camera_left(data);
		keypress = true;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
	{
		pan_camera_right(data);
		keypress = true;
	}
	if (keypress == true)
		raycaster(data);
}
