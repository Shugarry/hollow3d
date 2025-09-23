/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:38:14 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/23 18:33:02 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	valid_component(t_map *map, int x, int y)
{
	if (map->grid[x][y] == '0' || map->grid[x][y] == 'C' || \
			map->grid[x][y] == 'P' || map->grid[x][y] == 'E')
		return (1);
	return (0);
}

void	render_c(t_game *game, int x, int y, t_collectible **c)
{
	(*c)->x = x;
	(*c)->y = y;
	*c = (*c)-> next;
	mlx_image_to_window(game->mlx, game->img->collectibles \
		, y * TILE_SIZE, x * TILE_SIZE);
}

void	init_render(t_game *game, int *x, int *y, t_collectible **c)
{
	while (*x < game->map->height)
	{
		*y = 0;
		while (*y < game->map->width)
		{
			if (valid_component(game->map, *x, *y))
				mlx_image_to_window(game->mlx, game->img->floor, \
				*y * TILE_SIZE, *x * TILE_SIZE);
			if (game->map->grid[*x][*y] == '1')
				mlx_image_to_window(game->mlx, game->img->wall, \
				*y * TILE_SIZE, *x * TILE_SIZE);
			if (game->map->grid[*x][*y] == 'C')
				render_c(game, (*x), (*y), c);
			if (game->map->grid[*x][*y] == 'E')
				mlx_image_to_window(game->mlx, game->img->exit, \
				*y * TILE_SIZE, *x * TILE_SIZE);
			if (game->map->grid[*x][*y] == 'P')
				assign_axis(game, (*x), (*y));
			(*y)++;
		}
		(*x)++;
	}
}

void	render_map(t_game *game)
{
	int				x;
	int				y;
	t_collectible	*c;

	c = game->c;
	x = 0;
	y = 0;
	init_render(game, &x, &y, &c);
	mlx_image_to_window(game->mlx, game->img->player, game->player_x \
	* TILE_SIZE, game->player_y * TILE_SIZE);
	update_moves(game);
}

void	assign_axis(t_game *game, int x, int y)
{
	game->player_x = y;
	game->player_y = x;
}
