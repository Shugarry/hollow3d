/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:12:20 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/23 18:32:14 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	flood_fill(t_map *map, char **grid, int x, int y)
{
	if (x < 0 || y < 0 || x >= map->width || y >= map->height)
		return ;
	if (grid[y][x] == '1' || grid[y][x] == 'V')
		return ;
	if (grid[y][x] == 'E')
		map->exit_found = 1;
	if (grid[y][x] == 'P')
		map->player_found = 1;
	grid[y][x] = 'V';
	flood_fill(map, grid, x + 1, y);
	flood_fill(map, grid, x - 1, y);
	flood_fill(map, grid, x, y + 1);
	flood_fill(map, grid, x, y - 1);
}

int	valid_path(t_map *map)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	map->exit_found = 0;
	find_player(&i, &j, map-> grid);
	flood_fill(map, map->grid, j, i);
	if (!map->exit_found)
		return (ft_printf("error\nexit not found\n"), 0);
	if (!map->player_found)
		return (ft_printf("Error\nPlayer not found\n"), 0);
	return (1);
}

int	check_walls(t_map *map)
{
	char	**grid;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	k = map->height;
	grid = map->grid;
	while (grid[i][j] && j < map->width - 1)
	{
		if (grid[i][j] != '1' || grid[k - 1][j] != '1')
			return (write(2, "Error\nWalls are not valid\n", 26), 0);
		j++;
	}
	j = 0;
	i = 0;
	k = map->width;
	while (i < map->height && grid[i][k - 1])
	{
		if (grid[i][0] != '1' || grid[i][k - 2] != '1')
			return (write(2, "Error\nwall is not valid\n", 24), 0);
		i++;
	}
	return (1);
}

int	validate_map(t_map **map, t_game **game, char *argv)
{
	if (!init_map(argv, map))
		return (free_grid((*map)->grid), 0);
	calculate_dimensions(*map);
	if (!is_rectangle(*map))
		return (0);
	if (!check_walls(*map))
		return (0);
	if (!valid_path(*map))
		return (0);
	free_grid((*map)->grid);
	free((*map)->grid);
	if (!init_map(argv, map))
		return (0);
	if (!init_components(*map, *game))
		return (0);
	return (1);
}
