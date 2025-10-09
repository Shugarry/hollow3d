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
		rcast->delta_dist_x = ft_fabs(1 / rcast->ray_dir_x);
	if (rcast->ray_dir_y == 0)
		rcast->delta_dist_y = INFINITY;
	else
		rcast->delta_dist_y = ft_fabs(1 / rcast->ray_dir_y);
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
	uint8_t *pixel;
	uint32_t color;

	pixel = texture->pixels + ((y * texture->width + x) << 2);
	color = rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
	return (color);
}

mlx_texture_t	*get_texture(t_data *data, double x, double y)
{
	mlx_texture_t	*texture;

	texture = NULL;
	if (data->raycast.side == 0)
	{
		if (x > 0)
			texture = data->textures.east;
		else
			texture = data->textures.west;
	}
	else
	{
		if (y > 0)
			texture = data->textures.south;
		else
			texture = data->textures.north;
	}
	return (texture);
}

void	texturize_walls(t_data *data, int x)
{
	t_raycast		*rcast;
	int				line;
	uint32_t		color;
	mlx_texture_t	*current_texture;

	rcast = &data->raycast;
	if (rcast->side == 0)
		rcast->wall_x = data->player.curr_y + rcast->perp_wall_dist * rcast->ray_dir_y;
	else
		rcast->wall_x = data->player.curr_x + rcast->perp_wall_dist * rcast->ray_dir_x;
	current_texture = get_texture(data, rcast->ray_dir_x, rcast->ray_dir_y);
	rcast->wall_x -= ft_floor(rcast->wall_x);
	rcast->tex_x = (int)(rcast->wall_x * (double)current_texture->width);
	rcast->tex_step = 1.0 * current_texture->width / rcast->line_height;
	rcast->tex_pos = (rcast->draw_start - WIN_HEIGHT / 2 + rcast->line_height / 2) * \
		rcast->tex_step;
	line = rcast->draw_start;
	while (line < rcast->draw_end)
	{
		rcast->tex_y = (int)rcast->tex_pos & (current_texture->height - 1);
		rcast->tex_pos += rcast->tex_step;
		color = get_texture_pixel(current_texture, rcast->tex_x, rcast->tex_y);
		mlx_put_pixel(data->canvas, x, line, color);
		line++;
	}
}

static void	draw_walls(t_data *data, int x)
{
	t_raycast	*rcast;
	int			line;
	uint32_t		color;
	mlx_texture_t	*current_texture;

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
		mlx_put_pixel(data->canvas, x, line, data->textures.ceiling_color);
		line++;
	}
	texturize_walls(data, x);
	line = rcast->draw_end;
	while (line <= WIN_HEIGHT)
	{
		mlx_put_pixel(data->canvas, x, line, data->textures.floor_color);
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
