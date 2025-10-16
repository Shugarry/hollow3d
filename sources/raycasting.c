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

void	draw_floor_ceiling(t_data *data)
{
	mlx_texture_t	*current_texture;
	t_raycast		*r;
	uint32_t		color;
	int				y;
	int				x;

	y = WIN_HEIGHT - 1;
	current_texture = data->textures.floor;
	while (y > WIN_HEIGHT / 2)
	{
		r = &data->raycast;
		r->ray_dir_x_left = r->dir_x - r->plane_x;
		r->ray_dir_y_left = r->dir_y - r->plane_y;
		r->ray_dir_x_right = r->dir_x + r->plane_x;
		r->ray_dir_y_right = r->dir_y + r->plane_y;
		r->pos_y = y - WIN_HEIGHT / 2;
		r->pos_z = 0.5 * WIN_HEIGHT;
		r->row_distance = r->pos_z / r->pos_y;
		r->floor_step_x = r->row_distance * \
			(r->ray_dir_x_right - r->ray_dir_x_left) / WIN_WIDTH;
		r->floor_step_y = r->row_distance * \
			(r->ray_dir_y_right - r->ray_dir_y_left) / WIN_WIDTH;
		r->floor_x = data->player.curr_x + r->row_distance * \
			r->ray_dir_x_left;
		r->floor_y = data->player.curr_y + r->row_distance * \
			r->ray_dir_y_left;
		x = 0;
		while (x < WIN_WIDTH)
		{
			r->cell_x = (int)r->floor_x;
			r->cell_y = (int)r->floor_y;

			r->f_tex_x = (int)(current_texture->width * \
				(r->floor_x - r->cell_x)) & (current_texture->width - 1);
			r->f_tex_y = (int)(current_texture->height * \
				(r->floor_y - r->cell_y)) & (current_texture->height - 1);
			r->floor_x += r->floor_step_x;
			r->floor_y += r->floor_step_y;
			color = get_texture_pixel(current_texture, r->f_tex_x, \
							 r->f_tex_y);
			mlx_put_pixel(data->canvas, x, y, color);
			mlx_put_pixel(data->canvas, x, WIN_HEIGHT - y - 1, color);
			++x;
		}
		y--;
	}
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
		x++;
	}
}
