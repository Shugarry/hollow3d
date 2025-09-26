/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:17:24 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/26 18:59:21 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libraries/libft/libft.h"
#include <stdlib.h>

int	is_map_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '\0')
		return (0);
	while (line[i])
	{
		if (line[i] != '0' && line[i] != '1' && \
				line[i] != 'N' && line[i] != 'S' && \
				line[i] != 'E' && line[i] != 'W' && \
				line[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

void	calculate_height(t_map *map, int start)
{
	int		i;
	char	**grid;

	grid = map->grid;
	i = 0;
	start = 0;
	while (grid[i] && is_map_line(grid[i]))
		i++;
	while (grid[i])
	{
		if (is_map_line(grid[i]))
		{
			error_and_free("Map must not be seperated", map);
		}
		i++;
	}
	map->height = i;
}

int	check_map(t_map *map)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	find_elements(&i, map);
	remove_elements(&map->grid, i);
	remove_elements(&map->map, i);
	calculate_height(map, i);
	find_player(&j, map);
	if (!map->player_found)
		error_and_free("Player not found", map);
	if (!flood_fill(map, map->player->x, map->player->y))
		error_and_free("Map_invalid", map);
	return (1);
}

void	print_grid(char **grid)
{
	int	i;

	i = 0;
	if (!grid)
		return ;
	while (grid[i])
	{
		printf("%s", grid[i]);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_map	*map;
	int		fd;

	(void) map;
	fd = open(argv[1], O_RDONLY);
	if (fd == -1 || argc != 2)
		return (printf("ERROR\n"), 0);
	init_map_vars(&map);
	init_map(argv[1], &map);
	map->elements_grid = ft_strdup_double(map->grid);
	check_map(map);
	cleanup(map);
	return (0);
}
