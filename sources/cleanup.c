/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:12:35 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/23 18:33:02 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	free_collectibles(t_collectible **collectibles)
{
	t_collectible	*tmp;

	while ((*collectibles))
	{
		tmp = *collectibles;
		*collectibles = (*collectibles)->next;
		free(tmp);
	}
}

void	cleanup(t_game **game)
{
	free_grid((*game)->map->grid);
	free((*game)->img);
	free((*game)->c);
	free((*game)->map->grid);
	free((*game)->map);
	free((*game));
}

void	init_exit(t_game *game)
{
	int		x;
	int		y;
	char	**grid;

	grid = game->map->grid;
	x = game->img->player->instances[0].x / TILE_SIZE;
	y = game->img->player->instances[0].y / TILE_SIZE;
	if (grid[y][x] == 'E' && game->collected >= game->total_collectibles)
	{
		mlx_terminate(game->mlx);
		free_grid(game->map->grid);
		free(game->img);
		free_collectibles(&game->c);
		free(game->map->grid);
		free(game->map);
		free(game);
		exit(0);
	}
}

void	close_game(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	ft_printf("Exiting game...\n");
	mlx_terminate(game->mlx);
	free_grid(game->map->grid);
	free(game);
}

void	free_all(t_game *game, t_map *map)
{
	free_grid(map->grid);
	free(game->img);
	free_collectibles(&game->c);
	free(game);
	free(map->grid);
	free(map);
}
