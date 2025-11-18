/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting_walls.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 17:10:25 by frey-gal          #+#    #+#             */
/*   Updated: 2025/11/18 18:05:36 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

uint32_t	get_tex_pixel(mlx_texture_t *texture, int x, int y, int darken)
{
	uint8_t		*pixel;
	uint32_t	color;

	pixel = texture->pixels + ((y * texture->width + x) << 2);
	color = rgba(pixel[0], pixel[1], pixel[2], (pixel[3] << darken));
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
	t_raycast	*r;

	r = &data->raycast;
	if (r->side == 0)
		r->wall_x = data->player.curr_y + r->perp_wall_dist * r->ray_dir_y;
	else
		r->wall_x = data->player.curr_x + r->perp_wall_dist * r->ray_dir_x;
}

static void	texturize_walls(t_data *data, int x)
{
	t_raycast		*r;
	int				line;
	uint32_t		color;
	mlx_texture_t	*current_texture;

	r = &data->raycast;
	get_wall_x(data);
	current_texture = get_texture(data, r->ray_dir_x, r->ray_dir_y);
	r->wall_x -= floor(r->wall_x);
	if ((r->side == 0 && r->ray_dir_x <= 0) || \
		(r->ray_dir_y > 0 && r->side == 1))
		r->wall_x = 1 - r->wall_x;
	r->tex_x = (int)(r->wall_x * (double)current_texture->width);
	r->tex_step = 1.0 * current_texture->width / r->line_height;
	r->tex_pos = (r->draw_start - WIN_HEIGHT / 2 + r->line_height / 2) * \
		r->tex_step;
	line = r->draw_start;
	while (line < r->draw_end)
	{
		r->tex_y = (int)r->tex_pos & (current_texture->height - 1);
		r->tex_pos += r->tex_step;
		color = get_tex_pixel(current_texture, r->tex_x, r->tex_y, false);
		if (color)
			mlx_put_pixel(data->canvas, x, line, color);
		line++;
	}
}

void	draw_walls(t_data *data, int x)
{
	t_raycast	*r;

	r = &data->raycast;
	r->line_height = (int)(WIN_HEIGHT / r->perp_wall_dist);
	r->draw_start = -r->line_height / 2 + WIN_HEIGHT / 2;
	if (r->draw_start < 0)
		r->draw_start = 0;
	if (r->draw_start >= WIN_HEIGHT)
		r->draw_start = WIN_HEIGHT - 1;
	r->draw_end = r->line_height / 2 + WIN_HEIGHT / 2;
	if (r->draw_end < 0)
		r->draw_end = 0;
	if (r->draw_end >= WIN_HEIGHT)
		r->draw_end = WIN_HEIGHT - 1;
	texturize_walls(data, x);
}
