/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_walls.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:10:25 by frey-gal          #+#    #+#             */
/*   Updated: 2025/10/14 16:29:17 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static uint32_t	get_texture_pixel(mlx_texture_t *texture, int x, int y)
{
	uint8_t		*pixel;
	uint32_t	color;

	pixel = texture->pixels + ((y * texture->width + x) << 2);
	color = rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
	return (color);
}

static mlx_texture_t	*get_texture(t_data *data, double x, double y)
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

static void	get_wall_x(t_data *data)
{
	t_raycast	*rc;

	rc = &data->raycast;
	if (rc->side == 0)
		rc->wall_x = data->player.curr_y + rc->perp_wall_dist * rc->ray_dir_y;
	else
		rc->wall_x = data->player.curr_x + rc->perp_wall_dist * rc->ray_dir_x;
}

static void	texturize_walls(t_data *data, int x)
{
	t_raycast		*rc;
	int				line;
	uint32_t		color;
	mlx_texture_t	*current_texture;

	rc = &data->raycast;
	get_wall_x(data);
	current_texture = get_texture(data, rc->ray_dir_x, rc->ray_dir_y);
	rc->wall_x -= floor(rc->wall_x);
	if ((rc->side == 0 && rc->ray_dir_x <= 0) || \
		(rc->ray_dir_y > 0 && rc->side == 1))
		rc->wall_x = 1 - rc->wall_x;
	rc->tex_x = (int)(rc->wall_x * (double)current_texture->width);
	rc->tex_step = 1.0 * current_texture->width / rc->line_height;
	rc->tex_pos = (rc->draw_start - WIN_HEIGHT / 2 + rc->line_height / 2) * \
		rc->tex_step;
	line = rc->draw_start;
	while (line < rc->draw_end)
	{
		rc->tex_y = (int)rc->tex_pos & (current_texture->height - 1);
		rc->tex_pos += rc->tex_step;
		color = get_texture_pixel(current_texture, rc->tex_x, rc->tex_y);
		mlx_put_pixel(data->canvas, x, line, color);
		line++;
	}
}

void	draw_walls(t_data *data, int x)
{
	t_raycast	*rcast;
	int			line;

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
		mlx_put_pixel(data->canvas, x, line++, data->textures.ceiling_color);
	texturize_walls(data, x);
	line = rcast->draw_end;
	while (line <= WIN_HEIGHT)
		mlx_put_pixel(data->canvas, x, line++, data->textures.floor_color);
}
