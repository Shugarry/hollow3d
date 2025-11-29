/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:07:17 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/29 18:18:53 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

bool	is_valid(char c)
{
	if (c == '1' || c == '0' || c == ' ' \
		|| c == 'v' || c == 'N' || c == 'S' \
		|| c == 'E' || c == 'W' || c == 'e')
		return (true);
	return (false);
}

char	set_map_char(char **map, int x, int y, int height)
{
	int	len;

	if (x < 0 || x >= height)
		return (' ');
	len = ft_strlen(map[x]);
	if (x < 0 || x >= len)
		return (' ');
	return (map[x][y]);
}

int	flood_fill(t_data *data, int x, int y)
{
	char	c;

	c = set_map_char(data->parsing.grid, x, y, data->parsing.height);
	if (c == ' ')
		return (0);
	if (c == '1' || c == 'v')
		return (1);
	if (!is_valid(c))
		return (0);
	data->parsing.grid[x][y] = 'v';
	if (!flood_fill(data, x + 1, y))
		return (0);
	if (!flood_fill(data, x - 1, y))
		return (0);
	if (!flood_fill(data, x, y + 1))
		return (0);
	if (!flood_fill(data, x, y - 1))
		return (0);
	return (1);
}
