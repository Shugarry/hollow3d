/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 20:30:08 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/11 15:15:11 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_in_circle(int x, int y, int radius)
{
	int	dx;
	int	dy;

	dx = x - MINIMAP_CENTER_X;
	dy = y - MINIMAP_CENTER_Y;
	return (dx * dx + dy * dy <= radius * radius);
}

void	draw_line(mlx_image_t *img, t_data *data, int x1, int y1)
{
	int			i;
	uint32_t	*pixel;

	data->line.dx = x1 - MINIMAP_CENTER_X;
	data->line.dy = y1 - MINIMAP_CENTER_Y;
	data->line.steps = fmax(abs(data->line.dx), abs(data->line.dy));
	data->line.x_inc = data->line.dx / (float)data->line.steps;
	data->line.y_inc = data->line.dy / (float)data->line.steps;
	data->line.x = MINIMAP_CENTER_X;
	data->line.y = MINIMAP_CENTER_Y;
	i = 0;
	while (i <= data->line.steps)
	{
		if (data->line.x >= 0 && data->line.x < img->width && data->line.y \
				>= 0 && data->line.y < img->height)
		{
			pixel = (uint32_t *)(img->pixels + ((int)data->line.y * img->width \
						+ (int)data->line.x) * sizeof(uint32_t));
			*pixel = PLAYER_DIR_COLOR;
		}
		data->line.x += data->line.x_inc;
		data->line.y += data->line.y_inc;
		i++;
	}
}

void	update_minimap(t_data *data)
{
	memset(data->mini->pixels, 0, data->mini->width * \
			data->mini->height * sizeof(uint32_t));
	data->circle.center_x = MINIMAP_CENTER_X;
	data->circle.center_y = MINIMAP_CENTER_Y;
	draw_minimap_circle(data->mini, data, MINI_RADIUS, 0x333333FF);
	draw_minimap_layout(data);
	draw_player_on_minimap(data);
	draw_minimap_circle_border(data);
	draw_enemies_on_minimap(data);
}

void	init_mini(t_data *data)
{
	int	size;

	size = MINIMAP_RADIUS * 2;
	data->mini = mlx_new_image(data->mlx, size, size);
	mlx_image_to_window(data->mlx, data->mini, 0, 0);
	data->circle.center_x = MINIMAP_CENTER_X;
	data->circle.center_y = MINIMAP_CENTER_Y;
	draw_minimap_circle(data->mini, data, MINIMAP_RADIUS, 0x333333FF);
}
