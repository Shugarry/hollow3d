/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_components.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:27:58 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/23 18:33:02 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	check_components(t_map *map, int *exit, int *collectables, int *s_pos)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (map->grid[i])
	{
		while (map->grid[i][j])
		{
			if (map->grid[i][j] == 'E')
				(*exit)++;
			else if (map->grid[i][j] == 'C')
				(*collectables)++;
			else if (map->grid[i][j] == 'P')
				(*s_pos)++;
			j++;
		}
		j = 0;
		i++;
	}
}

int	init_components(t_map *map, t_game *game)
{
	int	exit;
	int	collectibles;
	int	starting_pos;

	exit = 0;
	collectibles = 0;
	starting_pos = 0;
	check_components(map, &exit, &collectibles, &starting_pos);
	if (exit != 1 || collectibles < 1 || starting_pos != 1)
		return (write(2, "Error in components\n", 20), 0);
	else
	{
		game->total_collectibles = collectibles;
		return (1);
	}
}

void	find_player(int *i, int *j, char **grid)
{
	while (grid[(*i)])
	{
		while (grid[(*i)][(*j)])
		{
			if (grid[(*i)][(*j)] == 'P')
				return ;
			(*j)++;
		}
		(*i)++;
		(*j) = 0;
	}
}

bool	is_valid_path(char **grid, int x, int y, int *collectibles)
{
	if (x < 0 || y < 0 || !grid[x] || grid[x][y] == '1' || grid[x][y] == 'X')
		return (false);
	if (grid[x][y] == 'C')
		(*collectibles)++;
	if (grid[x][y] == 'E')
	{
		return (true);
	}
	grid[x][y] = 'X';
	if (is_valid_path(grid, x + 1, y, collectibles) == true)
		return (true);
	else if (is_valid_path(grid, x - 1, y, collectibles) == true)
		return (true);
	else if (is_valid_path(grid, x, y + 1, collectibles) == true)
		return (true);
	else if (is_valid_path(grid, x, y - 1, collectibles) == true)
		return (true);
	return (false);
}

void	init_player_pos(t_game *game)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	find_player(&x, &y, game->map->grid);
	game->img->player->instances[0].x = x * TILE_SIZE;
	game->img->player->instances[0].y = y * TILE_SIZE;
}
