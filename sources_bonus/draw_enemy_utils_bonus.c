/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_enemy_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 23:07:57 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/10 23:07:58 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	draw_enemy(t_data *data, t_enemy *enemy)
{
	t_sprite_data	sp;

	get_sprite_transform(data, enemy, &data->enemy_vars.transform_x, \
			&data->enemy_vars.transform_y);
	if (data->enemy_vars.transform_y <= 0)
		return ;
	get_sprite_dimensions(data->enemy_vars.transform_x, \
			data->enemy_vars.transform_y, &sp.sprite_screen_x, \
			&sp.sprite_height);
	sp.sprite_width = sp.sprite_height;
	sp.transform_y = data->enemy_vars.transform_y;
	get_draw_bounds(sp.sprite_screen_x, sp.sprite_width, \
			&data->enemy_vars.draw_start_x, &data->enemy_vars.draw_end_x);
	get_draw_bounds_y(sp.sprite_height, &sp.draw_start_y, &sp.draw_end_y);
	data->enemy_vars.stripe = data->enemy_vars.draw_start_x;
	while (data->enemy_vars.stripe < data->enemy_vars.draw_end_x)
	{
		draw_sprite_column(data, enemy, data->enemy_vars.stripe, &sp);
		data->enemy_vars.stripe++;
	}
}

void	get_draw_bounds(int sprite_screen_x, int sprite_size, \
		int *start_x, int *end_x)
{
	*start_x = -sprite_size / 2 + sprite_screen_x;
	if (*start_x < 0)
		*start_x = 0;
	*end_x = sprite_size / 2 + sprite_screen_x;
	if (*end_x >= WIN_WIDTH)
		*end_x = WIN_WIDTH - 1;
}

void	get_draw_bounds_y(int sprite_size, int *start_y, int *end_y)
{
	*start_y = -sprite_size / 2 + WIN_HEIGHT / 2;
	if (*start_y < 0)
		*start_y = 0;
	*end_y = sprite_size / 2 + WIN_HEIGHT / 2;
	if (*end_y >= WIN_HEIGHT)
		*end_y = WIN_HEIGHT - 1;
}

void	draw_sprite_pixel(t_data *data, t_enemy *enemy, int stripe, int y)
{
	uint8_t		*pixel;
	uint32_t	color;

	if (data->textures.tex_x >= 0 && data->textures.tex_x < \
			(int)enemy->texture->width && \
			data->textures.tex_y >= 0 && data->textures.tex_y \
			< (int)enemy->texture->height)
	{
		pixel = enemy->texture->pixels + ((data->textures.tex_y * \
					enemy->texture->width + data->textures.tex_x) << 2);
		if (pixel[3] == 0)
			return ;
		color = rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
		mlx_put_pixel(data->canvas, stripe, y, color);
	}
}

void	draw_sprite_column(t_data *data, t_enemy *enemy, int stripe, \
		t_sprite_data *sp)
{
	int	y;
	int	d;

	data->textures.tex_x = (int)((stripe - (-sp->sprite_width / 2 \
					+ sp->sprite_screen_x)) * \
			enemy->texture->width / sp->sprite_width);
	if (sp->transform_y >= data->wall_distances[stripe])
		return ;
	y = sp->draw_start_y;
	while (y < sp->draw_end_y)
	{
		d = y - WIN_HEIGHT / 2 + sp->sprite_height / 2;
		data->textures.tex_y = d * enemy->texture->height / sp->sprite_height;
		draw_sprite_pixel(data, enemy, stripe, y);
		y++;
	}
}
