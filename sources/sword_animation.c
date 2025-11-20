/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sword_animation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/10 11:16:04 by frey-gal          #+#    #+#             */
/*   Updated: 2025/11/20 21:39:38 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"


// void check_if_dead(t_data *data)
// {
// 	int i;
// 	i = 0;
// 	while (i < data->enemy_count)
//     {
//         if (data->enemies[i].alive && data->enemies[i].in_range)
// 			data->enemies[i].alive = false;
//         i++;
//     }
// }

void check_if_dead(t_data *data)
{
    int i;
    int closest_enemy;
    double closest_distance;
    double dot_product;
    double enemy_dx;
    double enemy_dy;

    closest_enemy = -1;
    closest_distance = 999999.0;
    i = 0;
    while (i < data->enemy_count)
    {
        if (data->enemies[i].alive && data->enemies[i].in_range)
        {
            enemy_dx = data->enemies[i].x - data->player.curr_x;
            enemy_dy = data->enemies[i].y - data->player.curr_y;
            dot_product = enemy_dx * data->raycast.dir_x + enemy_dy * data->raycast.dir_y;
            if (dot_product > 0 && data->enemies[i].distance < closest_distance)
            {
                closest_distance = data->enemies[i].distance;
                closest_enemy = i;
            }
        }
        i++;
    }
    if (closest_enemy != -1 && !data->s_animation && data->sword_hit == 0)
    {
        data->enemies[closest_enemy].alive = false;
        printf("Killed enemy %d (distance: %.2f)\n", closest_enemy, closest_distance);
		data->s_animation = 1;
        data->sword_hit = 1;
    }
}
void	draw_sword_frames(t_data *data)
{
	//data->sword_hit = 1;
	//data->s_animation = 1;
	if (data->animation.frame_num >= 0 && data->animation.frame_num < 30)
		img_to_window_scaled(data, data->animation.sword[1], 0, 0);
	if (data->animation.frame_num >= 30 && data->animation.frame_num < 50)
	{
		img_to_window_scaled(data, data->animation.sword[2], 0, 0);
		printf("gets here\n");
        // if (data->sword_hit == 0)
		    check_if_dead(data);
        // data->sword_hit = 0;
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
	data->s_animation = 0;
	//data->sword_hit = 0;
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
    data->sword_hit = 0;
}
