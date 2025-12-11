/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:18:01 by frey-gal          #+#    #+#             */
/*   Updated: 2025/10/09 17:10:36 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	cast_rays(t_data *data, int x)
{
	t_raycast	*rcast;

	rcast = &data->raycast;
	rcast->camera_x = 2 * x / (double)WIN_WIDTH - 1;
	rcast->ray_dir_x = rcast->dir_x + rcast->plane_x * rcast->camera_x;
	rcast->ray_dir_y = rcast->dir_y + rcast->plane_y * rcast->camera_x;
	rcast->map_x = (int)data->player.curr_x;
	rcast->map_y = (int)data->player.curr_y;
	if (rcast->ray_dir_x == 0)
		rcast->delta_dist_x = INFINITY;
	else
		rcast->delta_dist_x = fabs(1 / rcast->ray_dir_x);
	if (rcast->ray_dir_y == 0)
		rcast->delta_dist_y = INFINITY;
	else
		rcast->delta_dist_y = fabs(1 / rcast->ray_dir_y);
	rcast->hit = false;
}

static void	step_in_dir(t_data *data)
{
	t_raycast	*rc;
	t_player	*player;

	rc = &data->raycast;
	player = &data->player;
	if (rc->ray_dir_x < 0)
	{
		rc->step_x = -1;
		rc->side_dist_x = (player->curr_x - rc->map_x) * rc->delta_dist_x;
	}
	else
	{
		rc->step_x = 1;
		rc->side_dist_x = (rc->map_x + 1.0 - player->curr_x) * rc->delta_dist_x;
	}
	if (rc->ray_dir_y < 0)
	{
		rc->step_y = -1;
		rc->side_dist_y = (player->curr_y - rc->map_y) * rc->delta_dist_y;
	}
	else
	{
		rc->step_y = 1;
		rc->side_dist_y = (rc->map_y + 1.0 - player->curr_y) * rc->delta_dist_y;
	}
}

static void	ray_find_wall(t_data *data)
{
	t_raycast	*rcast;

	rcast = &data->raycast;
	while (rcast->hit == false)
	{
		if (rcast->side_dist_x < rcast->side_dist_y)
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
		if (data->map[rcast->map_y][rcast->map_x] == '1')
			rcast->hit = true;
	}
	if (rcast->side == 0)
		rcast->perp_wall_dist = (rcast->side_dist_x - rcast->delta_dist_x);
	else
		rcast->perp_wall_dist = (rcast->side_dist_y - rcast->delta_dist_y);
}

void	raycaster(t_data *data)
{
	int	x;

	x = 0;
	while (x < WIN_WIDTH)
	{
		cast_rays(data, x);
		step_in_dir(data);
		ray_find_wall(data);
		draw_walls(data, x);
		x++;
	}
}
