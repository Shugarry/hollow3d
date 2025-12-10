/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemies.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 14:05:31 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/10 22:41:36 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	count_enemies(t_data *data)
{
	int	count;
	int	y;
	int	x;

	count = 0;
	y = 0;
	while (y < data->parsing.height)
	{
		x = 0;
		while (data->map[y][x])
		{
			if (data->map[y][x] == 'e')
				count++;
			x++;
		}
		y++;
	}
	return (count);
}

void	place_enemies(t_data *data)
{
	int	y;
	int	x;
	int	enemy_index;

	enemy_index = 0;
	y = 0;
	while (y < data->parsing.height)
	{
		x = 0;
		while (data->map[y][x])
		{
			if (data->map[y][x] == 'e')
			{
				data->enemies[enemy_index].x = x + 0.5;
				data->enemies[enemy_index].y = y + 0.5;
				data->enemies[enemy_index].alive = true;
				enemy_index++;
			}
			x++;
		}
		y++;
	}
}

void	init_enemies(t_data *data)
{
	int				i;
	mlx_texture_t	*enemy_tex;

	i = 0;
	data->enemy_count = count_enemies(data);
	if (data->enemy_count == 0)
	{
		data->enemies = NULL;
		return ;
	}
	data->enemies = malloc(sizeof(t_enemy) * data->enemy_count);
	if (!data->enemies)
		exit(1);
	place_enemies(data);
	enemy_tex = mlx_load_png("resources/skeleton_w.png");
	if (!enemy_tex)
		clean_exit(data, "Could not lead enemy texture", 1);
	while (i < data->enemy_count)
		data->enemies[i++].texture = enemy_tex;
}

void	calculate_enemy_distance(t_data *data, int i, t_enemy_vars *vars)
{
	double	dir_x;
	double	dir_y;

	vars->dx = data->enemies[i].x - data->player.curr_x;
	vars->dy = data->enemies[i].y - data->player.curr_y;
	data->enemies[i].distance = sqrt(vars->dx * vars->dx + vars->dy * vars->dy);
	dir_x = -vars->dx / data->enemies[i].distance;
	dir_y = -vars->dy / data->enemies[i].distance;
	vars->new_x = data->enemies[i].x + dir_x * vars->move_speed;
	vars->new_y = data->enemies[i].y + dir_y * vars->move_speed;
}

void	check_if_can_move(t_data *data, int i, t_enemy_vars *vars)
{
	int		j;
	double	dx;
	double	dy;

	j = 0;
	vars->can_move = 1;
	if (data->map[(int)vars->new_y][(int)vars->new_x] == '1')
		vars->can_move = 0;
	while (j < data->enemy_count && vars->can_move)
	{
		if (j != i && data->enemies[j].alive)
		{
			dx = vars->new_x - data->enemies[j].x;
			dy = vars->new_y - data->enemies[j].y;
			if (sqrt(dx * dx + dy * dy) < 1.0)
				vars->can_move = 0;
		}
		j++;
	}
}

void	update_enemy_pos(t_data *data, int i, t_enemy_vars *vars)
{
	if (data->enemies[i].distance > 2.0)
	{
		data->enemies[i].in_range == 0;
		check_if_can_move(data, i, vars);
		if (vars->can_move)
		{
			data->enemies[i].x = vars->new_x;
			data->enemies[i].y = vars->new_y;
		}
	}
	else if (data->enemies[i].distance < 2.0)
	{
		data->enemies[i].in_range = 1;
		data->player.dead = 1;
		if (data->enemies[i].distance < 0.5)
			clean_exit(data, "player died\n", 1);
	}
}

void	update_enemies(t_data *data)
{
	int				i;
	t_enemy_vars	*vars;

	vars = &data->enemy_vars;
	i = 0;
	while (i < data->enemy_count)
	{
		if (data->enemies[i].alive)
		{
			calculate_enemy_distance(data, i, vars);
			vars->move_speed = 0.05;
			update_enemy_pos(data, i, vars);
			if (data->enemies[i].distance > 2.0)
				data->enemies[i].in_range = 0;
		}
		i++;
	}
}

void	sort_enemies(t_data *data)
{
	int		i;
	int		j;
	t_enemy	temp;

	i = 0;
	while (i < data->enemy_count - 1)
	{
		j = 0;
		while (j < data->enemy_count - i - 1)
		{
			if (data->enemies[j].distance < data->enemies[j + 1].distance)
			{
				temp = data->enemies[j];
				data->enemies[j] = data->enemies[j + 1];
				data->enemies[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	get_sprite_transform(t_data *data, t_enemy *enemy, double *transform_x, \
double *transform_y)
{
	double	sprite_x;
	double	sprite_y;
	double	inv_det;

	sprite_x = enemy->x - data->player.curr_x;
	sprite_y = enemy->y - data->player.curr_y;
	inv_det = 1.0 / (data->raycast.plane_x * data->raycast.dir_y \
			- data->raycast.dir_x * data->raycast.plane_y);
	*transform_x = inv_det * (data->raycast.dir_y * sprite_x - \
			data->raycast.dir_x * sprite_y);
	*transform_y = inv_det * (-data->raycast.plane_y * sprite_x + \
			data->raycast.plane_x * sprite_y);
}

void	get_sprite_dimensions(double transform_x, \
		double transform_y, int *screen_x, int *height)
{
	*screen_x = (int)((WIN_WIDTH / 2) * (1 + transform_x / transform_y));
	*height = abs((int)(WIN_HEIGHT / transform_y));
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

void	check_distance_loop(t_data *data, int i, t_enemy_vars *vars)
{
	while (i < data->enemy_count)
	{
		if (data->enemies[i].alive && data->enemies[i].in_range)
		{
			vars->dx = data->enemies[i].x - data->player.curr_x;
			vars->dy = data->enemies[i].y - data->player.curr_y;
			vars->res = vars->dx * data->raycast.dir_x + \
			vars->dy * data->raycast.dir_y;
			if (vars->res > 0 && data->enemies[i].distance < \
					vars->closest_distance)
			{
				vars->closest_distance = data->enemies[i].distance;
				vars->closest_enemy = i;
			}
		}
		i++;
	}
}

void	check_if_dead(t_data *data)
{
	int				i;
	t_enemy_vars	vars;

	vars.closest_enemy = -1;
	vars.closest_distance = 999999.0;
	i = 0;
	check_distance_loop(data, i, &vars);
	if (vars.closest_enemy != -1 && !data->s_animation && data->sword_hit == 0)
	{
		data->enemies[vars.closest_enemy].alive = false;
		data->s_animation = 1;
		data->sword_hit = 1;
	}
}
