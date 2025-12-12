/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:22:20 by frey-gal          #+#    #+#             */
/*   Updated: 2025/10/07 16:25:29 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	look_north(t_data *data)
{
	data->raycast.dir_x = 0;
	data->raycast.dir_y = -1;
	data->raycast.plane_x = 0.66;
	data->raycast.plane_y = 0;
}

static void	look_south(t_data *data)
{
	data->raycast.dir_x = 0;
	data->raycast.dir_y = 1;
	data->raycast.plane_x = -0.66;
	data->raycast.plane_y = 0;
}

static void	look_east(t_data *data)
{
	data->raycast.dir_x = 1;
	data->raycast.dir_y = 0;
	data->raycast.plane_x = 0;
	data->raycast.plane_y = 0.66;
}

static void	look_west(t_data *data)
{
	data->raycast.dir_x = -1;
	data->raycast.dir_y = 0;
	data->raycast.plane_x = 0;
	data->raycast.plane_y = -0.66;
}

void	starting_vars(t_data *data)
{
	if (data->player.direction == 'N')
		look_north(data);
	else if (data->player.direction == 'S')
		look_south(data);
	else if (data->player.direction == 'E')
		look_east(data);
	else if (data->player.direction == 'W')
		look_west(data);
}
