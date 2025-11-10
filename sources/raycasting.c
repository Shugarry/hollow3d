/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:18:01 by frey-gal          #+#    #+#             */
/*   Updated: 2025/10/17 09:40:17 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	cast_rays(t_data *data, int x)
{
	t_raycast	*r;

	r = &data->raycast;
	r->camera_x = 2 * x / (double)WIN_WIDTH - 1;
	r->ray_dir_x = r->dir_x + r->plane_x * r->camera_x;
	r->ray_dir_y = r->dir_y + r->plane_y * r->camera_x;
	r->map_x = (int)data->player.curr_x;
	r->map_y = (int)data->player.curr_y;
	if (r->ray_dir_x == 0)
		r->delta_dist_x = INFINITY;
	else
		r->delta_dist_x = fabs(1 / r->ray_dir_x);
	if (r->ray_dir_y == 0)
		r->delta_dist_y = INFINITY;
	else
		r->delta_dist_y = fabs(1 / r->ray_dir_y);
	r->hit = false;
}

static void	step_in_dir(t_data *data)
{
	t_raycast	*r;
	t_player	*player;

	r = &data->raycast;
	player = &data->player;
	if (r->ray_dir_x < 0)
	{
		r->step_x = -1;
		r->side_dist_x = (player->curr_x - r->map_x) * r->delta_dist_x;
	}
	else
	{
		r->step_x = 1;
		r->side_dist_x = (r->map_x + 1.0 - player->curr_x) * r->delta_dist_x;
	}
	if (r->ray_dir_y < 0)
	{
		r->step_y = -1;
		r->side_dist_y = (player->curr_y - r->map_y) * r->delta_dist_y;
	}
	else
	{
		r->step_y = 1;
		r->side_dist_y = (r->map_y + 1.0 - player->curr_y) * r->delta_dist_y;
	}
}

static void	ray_find_wall(t_data *data)
{
	t_raycast	*r;

	r = &data->raycast;
	while (r->hit == false)
	{
		if (r->side_dist_x < r->side_dist_y)
		{
			r->side_dist_x += r->delta_dist_x;
			r->map_x += r->step_x;
			r->side = 0;
		}
		else
		{
			r->side_dist_y += r->delta_dist_y;
			r->map_y += r->step_y;
			r->side = 1;
		}
		if (data->map[r->map_y][r->map_x] == '1')
			r->hit = true;
	}
	if (r->side == 0)
		r->perp_wall_dist = (r->side_dist_x - r->delta_dist_x);
	else
		r->perp_wall_dist = (r->side_dist_y - r->delta_dist_y);
}

void	raycaster(t_data *data)
{
	int	x;

	x = 0;
	draw_floor_ceiling(data);
	while (x < WIN_WIDTH)
	{
		cast_rays(data, x);
		step_in_dir(data);
		ray_find_wall(data);
		draw_walls(data, x);
		data->wall_distances[x] = data->raycast.perp_wall_dist;
		x++;
	}
}
