/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 16:18:01 by frey-gal          #+#    #+#             */
/*   Updated: 2025/10/07 16:26:21 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	cast_rays(t_data *data, int x)
{
	t_raycast	*rcast;

	rcast = &data->raycast;
	rcast->camera_x = 2 * x / (double)WIN_WIDTH - 1;	// camera x is the xcoord on the camera plane that the current
														// xcoord on the screen represents so that left, middle and right
														// are -1, 0 and 1 respectively.
	rcast->ray_dir_x = rcast->dir_x + rcast->plane_x * rcast->camera_x; // direction vector for xcoords
	rcast->ray_dir_y = rcast->dir_y + rcast->plane_y * rcast->camera_x; // direction vector for ycoords
	rcast->map_x = (int)data->player.curr_x;	// Current x and y positions of the ray on the map array 
	rcast->map_y = (int)data->player.curr_y;
	if (rcast->ray_dir_x == 0)
		rcast->delta_dist_x = INFINITY;
	else
		rcast->delta_dist_x = fabs(1 / rcast->ray_dir_x);
	if (rcast->ray_dir_y == 0)
		rcast->delta_dist_y = INFINITY;
	else
		rcast->delta_dist_y = fabs(1 / rcast->ray_dir_y);
	rcast->hit = false; // was a wall hit?
}

static void	step_in_dir(t_data *data)
{
	t_raycast	*rcast;

	rcast = &data->raycast;
	if (rcast->ray_dir_x < 0)
	{
		rcast->step_x = -1;
		rcast->side_dist_x = (data->player.curr_x - rcast->map_x) * rcast->delta_dist_x;
	}
	else
	{
		rcast->step_x = 1;
		rcast->side_dist_x = (rcast->map_x + 1.0 - data->player.curr_x) * rcast->delta_dist_x;
	}
	if (rcast->ray_dir_y < 0)
	{
		rcast->step_y = -1;
		rcast->side_dist_y = (data->player.curr_y - rcast->map_y) * rcast->delta_dist_y;
	}
	else
	{
		rcast->step_y = 1;
		rcast->side_dist_y = (rcast->map_y + 1.0 - data->player.curr_y) * rcast->delta_dist_y;
	}
}

static void	ray_find_wall(t_data *data)
{
	t_raycast *rcast;

	rcast = &data->raycast;
	while(rcast->hit == false)
	{
		//jump to next map square, either in x-direction, or in y-direction
		if(rcast->side_dist_x < rcast->side_dist_y)
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
		if(data->map[rcast->map_y][rcast->map_x] == '1')
			rcast->hit = true;
	}
	if (rcast->side == 0)
		rcast->perp_wall_dist = (rcast->side_dist_x - rcast->delta_dist_x);
	else
		rcast->perp_wall_dist = (rcast->side_dist_y - rcast->delta_dist_y);
}

static void	draw_walls(t_data *data, int x)
{
	t_raycast *rcast;

	rcast = &data->raycast;
	rcast->line_height = (int)(WIN_HEIGHT / rcast->perp_wall_dist);
	rcast->draw_start = -rcast->line_height / 2 + WIN_HEIGHT / 2;
	if (rcast->draw_start < 0)
		rcast->draw_start = 0;
	if (rcast->draw_start >= WIN_HEIGHT)
		rcast->draw_start = WIN_HEIGHT - 1;
	rcast->draw_end = rcast->line_height / 2 + WIN_HEIGHT / 2;
	if (rcast->draw_end < 0)
		rcast->draw_end = 0;
	if (rcast->draw_end >= WIN_HEIGHT)
		rcast->draw_end = WIN_HEIGHT - 1;
	for (int l = 0; l <= rcast->draw_start; l++)
		mlx_put_pixel(data->canvas, x, l, rgba(0, 0, 0, 255));
	for (int l = rcast->draw_start; l <= rcast->draw_end; l++)
		mlx_put_pixel(data->canvas, x, l, get_color(rcast->side));
	for (int l = rcast->draw_end; l <= WIN_HEIGHT; l++)
		mlx_put_pixel(data->canvas, x, l, rgba(0, 0, 0, 255));
}

void	raycaster(t_data *data)
{
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		cast_rays(data, x);
		step_in_dir(data);
		ray_find_wall(data);
		draw_walls(data, x);
		// printf("rcast->perp_wall_dist %f\n", rcast->perp_wall_dist);
		// printf("rcast->line_height %i\n", rcast->line_height);
		// printf("rcast->draw_start %i, rcast->draw_end %i\n", rcast->draw_start, rcast->draw_end);
		// printf("map_x %d, map_y %d\n", rcast->map_x, rcast->map_y);
		// printf("player_x %f, player_y %f\n", data->player.curr_x, data->player.curr_y);
		// printf("\n");
	}
}
