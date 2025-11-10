/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 11:10:35 by frey-gal          #+#    #+#             */
/*   Updated: 2025/11/10 11:34:43 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	img_to_window_scaled(t_data *data, mlx_texture_t *texture, \
					double scale, int pos_x, int pos_y)
{
	uint32_t	color;
	int			i;
	int			j;
	double		step_x;
	double		step_y;

	step_x = (double)texture->width / (double)(texture->width * scale);
	step_y = (double)texture->height / (double)(texture->height * scale);
	i = 0;
	while (i < (texture->height * scale))
	{
		j = 0;
		while (j < (texture->width * scale))
		{
			color = get_tex_pixel(texture, (int)(j * step_x), \
					(int)(i * step_y), false);
			if (color)
				mlx_put_pixel(data->canvas, pos_x + j, pos_y + i, color);
			j++;
		}
		i++;
	}
}

void	fps_counter(t_data *data)
{
	char	fps_string[7];
	int		fps;

	fps = (int)(1.0 / data->raycast.frame_time);
	fps_string[0] = 'f';
	fps_string[1] = 'p';
	fps_string[2] = 's';
	fps_string[3] = ' ';
	fps_string[4] = '0' + fps / 10;
	fps_string[5] = '0' + fps % 10;
	fps_string[6] = '\0';
	data->raycast.old_time = data->raycast.time;
	data->raycast.time = get_time_seconds();
	data->raycast.frame_time = data->raycast.time - data->raycast.old_time ;
	img_to_window_scaled(data, data->textures.fps_ui, \
					0.25, WIN_WIDTH - data->textures.fps_ui->width * \
					0.25 - X_MARGIN, Y_MARGIN);
	mlx_delete_image(data->mlx, data->fps_str);
	data->fps_str = mlx_put_string(data->mlx, fps_string, \
					WIN_WIDTH - data->textures.fps_ui->width * \
					0.25 * 0.8 - X_MARGIN, Y_MARGIN * 10);
}
