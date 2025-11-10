/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sword_animation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 11:16:04 by frey-gal          #+#    #+#             */
/*   Updated: 2025/11/10 11:23:19 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	draw_sword_frames(t_data *data)
{
	if (data->animation.frame_num >= 0 && data->animation.frame_num < 30)
		img_to_window_scaled(data, data->animation.sword[1], \
				data->animation.sword[1]->width / WIN_WIDTH, 0, 0);
	if (data->animation.frame_num >= 30 && data->animation.frame_num < 50)
		img_to_window_scaled(data, data->animation.sword[2], \
				data->animation.sword[2]->width / WIN_WIDTH, 0, 0);
	if (data->animation.frame_num >= 50 && data->animation.frame_num < 70)
		img_to_window_scaled(data, data->animation.sword[3], \
				data->animation.sword[3]->width / WIN_WIDTH, 0, 0);
	if (data->animation.frame_num >= 70 && data->animation.frame_num < 100)
		img_to_window_scaled(data, data->animation.sword[4], \
				data->animation.sword[4]->width / WIN_WIDTH, 0, 0);
	if (data->animation.frame_num >= 100)
	{
		data->animation.in_animation = false;
		img_to_window_scaled(data, data->animation.sword[0], \
				data->animation.sword[0]->width / WIN_WIDTH, 0, 0);
	}
}

void	sword_animation(t_data *data)
{
	if (mlx_is_key_down(data->mlx, MLX_KEY_UP) && !data->animation.in_animation)
	{
		data->animation.in_animation = true;
		data->animation.frame_num = 0;
	}
	if (data->animation.in_animation)
		draw_sword_frames(data);
	else
		img_to_window_scaled(data, data->animation.sword[0], \
					data->animation.sword[0]->width / WIN_WIDTH, 0, 0);
}
