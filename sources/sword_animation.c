/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sword_animation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 11:16:04 by frey-gal          #+#    #+#             */
/*   Updated: 2025/11/12 19:25:51 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"


void check_if_dead(t_data *data)
{
	int i;
	i = 0;
	while (i < data->enemy_count)
    {
        if (data->enemies[i].alive && data->enemies[i].in_range)
			data->enemies[i].alive = false;
        i++;
    }
}
void	draw_sword_frames(t_data *data)
{
	data->sword_hit = 1;
	if (data->animation.frame_num >= 0 && data->animation.frame_num < 30)
		img_to_window_scaled(data, data->animation.sword[1], 0, 0);
	if (data->animation.frame_num >= 30 && data->animation.frame_num < 50)
	{
		img_to_window_scaled(data, data->animation.sword[2], 0, 0);
		printf("gets here\n");
		check_if_dead(data);
	}
	if (data->animation.frame_num >= 50 && data->animation.frame_num < 70)
		img_to_window_scaled(data, data->animation.sword[3], 0, 0);
	if (data->animation.frame_num >= 70 && data->animation.frame_num < 100)
		img_to_window_scaled(data, data->animation.sword[4], 0, 0);
	if (data->animation.frame_num >= 100)
	{
		data->animation.in_animation = false;
		img_to_window_scaled(data, data->animation.sword[0], 0, 0);
	}
	data->sword_hit = 0;
}

void	sword_animation(t_data *data)
{
	data->scale = data->animation.sword[1]->width / WIN_WIDTH;
	if (mlx_is_key_down(data->mlx, MLX_KEY_UP) && !data->animation.in_animation)
	{
		data->animation.in_animation = true;
		data->animation.frame_num = 0;
	}
	if (data->animation.in_animation)
		draw_sword_frames(data);
	else
		img_to_window_scaled(data, data->animation.sword[0], 0, 0);
}
