/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_floor_ceiling.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 09:38:59 by frey-gal          #+#    #+#             */
/*   Updated: 2025/10/17 09:40:19 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	init_scanlines(t_data *data, int y)
{
	t_raycast	*r;

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
}

static	void	draw_f_c_loop(t_data *data, int x, int y)
{
	t_raycast		*r;
	uint32_t		color;
	mlx_texture_t	*floor;
	mlx_texture_t	*ceiling;

	floor = data->textures.floor;
	ceiling = data->textures.ceiling;
	r = &data->raycast;
	r->cell_x = (int)r->floor_x;
	r->cell_y = (int)r->floor_y;
	r->f_tex_x = (int)(floor->width * \
		(r->floor_x - r->cell_x)) & (floor->width - 1);
	r->f_tex_y = (int)(floor->height * \
		(r->floor_y - r->cell_y)) & (floor->height - 1);
	r->floor_x += r->floor_step_x;
	r->floor_y += r->floor_step_y;
	color = get_tex_pixel(floor, r->f_tex_x, r->f_tex_y, 0);
	mlx_put_pixel(data->canvas, x, y, color);
	color = get_tex_pixel(ceiling, r->f_tex_x, r->f_tex_y, 0);
	mlx_put_pixel(data->canvas, x, WIN_HEIGHT - y - 1, color);
}

void	draw_floor_ceiling(t_data *data)
{
	int				y;
	int				x;

	y = WIN_HEIGHT - 1;
	while (y > WIN_HEIGHT / 2)
	{
		init_scanlines(data, y);
		x = 0;
		while (x < WIN_WIDTH)
		{
			draw_f_c_loop(data, x, y);
			++x;
		}
		x = 0;
		y--;
	}
}
