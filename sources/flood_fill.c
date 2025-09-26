/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:07:17 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/26 18:07:18 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libraries/libft/libft.h"

// 1, 0 space, v, N, S, E, W
int	is_valid(char c)
{
	if (c == '1' || c == '0' || c == ' ' \
		|| c == 'v' || c == 'N' || c == 'S' \
		|| c == 'E' || c == 'W')
		return (1);
	return (0);
}

char	set_map_char(char **map, int x, int y, int height)
{
	int	len;

	if (x < 0 || x >= height)
		return (' ');
	len = strlen(map[x]);
	if (x < 0 || x >= len)
		return (' ');
	return (map[x][y]);
}

int	flood_fill(t_map *map, int x, int y)
{
	char	c;

	c = set_map_char(map->grid, x, y, map->height);
	if (c == ' ')
	{
		return (0);
	}
	if (c == '1' || c == 'v')
		return (1);
	if (!is_valid(c))
		return (0);
	map->grid[x][y] = 'v';
	if (!flood_fill(map, x + 1, y))
		return (0);
	if (!flood_fill(map, x - 1, y))
		return (0);
	if (!flood_fill(map, x, y + 1))
		return (0);
	if (!flood_fill(map, x, y - 1))
		return (0);
	return (1);
}
