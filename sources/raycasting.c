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

uint32_t	get_texture_pixel(mlx_texture_t *texture, int x, int y)
{
	uint8_t *pixel = texture->pixels + (y * texture->width + x) * texture->bytes_per_pixel;

	uint32_t color = rgba(pixel[0], pixel[1], pixel[2], pixel[3]);

	return color;
}

static void	draw_walls(t_data *data, int x)
{
	t_raycast	*rcast;
	int			line;
	double wall_x;
	int tex_x, tex_y;
	double tex_step, tex_pos;
	uint32_t  col;

	line = 0;
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
	while (line < rcast->draw_start)
	{
		mlx_put_pixel(data->canvas, x, line, rgba(128, 0, 0, 255));
		line++;
	}
	if (rcast->side == 0)
		wall_x = data->player.curr_y + rcast->perp_wall_dist * rcast->ray_dir_y;
	else
		wall_x = data->player.curr_x + rcast->perp_wall_dist * rcast->ray_dir_x;
	wall_x -= floor(wall_x);
	tex_x = (int)(wall_x * (double)data->textures.north->width);
	tex_step = 1.0 * data->textures.north->width / rcast->line_height;
	tex_pos = (rcast->draw_start - WIN_HEIGHT / 2 + rcast->line_height / 2) * tex_step;
	for (int y = rcast->draw_start; y < rcast->draw_end; y++)
	{
		tex_y = (int)tex_pos & (data->textures.north->height - 1);
		tex_pos += tex_step;
		col = get_texture_pixel(data->textures.north, tex_x, tex_y);
		//printf("col %d\n", col);
		//printf("x %i y %i\n", x, y);
		mlx_put_pixel(data->canvas, x, y, col);
	}
	// while (line < rcast->draw_end)
	// {
	// 	mlx_put_pixel(data->canvas, x, line, get_color(rcast->side));
	// 	line++;
	// }
	line = rcast->draw_end;
	while (line <= WIN_HEIGHT)
	{
		mlx_put_pixel(data->canvas, x, line, rgba(128, 0, 0, 255));
		line++;
	}
}

void	raycaster(t_data *data)
{
	for (int x = 0; x < WIN_WIDTH; x++)
	{
		cast_rays(data, x);
		step_in_dir(data);
		ray_find_wall(data);
		draw_walls(data, x);
	}
}
