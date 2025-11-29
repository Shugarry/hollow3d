/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frey-gal <frey-gal@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:15:21 by frey-gal          #+#    #+#             */
/*   Updated: 2025/10/14 16:15:35 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

bool	is_map_line(char *line)
{
	int	i;

	i = 0;
	while (line && line[i] == ' ')
		i++;
	if (line[i] == '\0')
		return (false);
	while (line[i])
	{
		if (line[i] != '0' && line[i] != '1' && \
				line[i] != 'N' && line[i] != 'S' && \
				line[i] != 'E' && line[i] != 'W' && \
				line[i] != ' ' && \
				line[i] != 'e')
			return (false);
		i++;
	}
	return (true);
}

void	calculate_height(t_data *data)
{
	int		i;
	char	**grid;

	grid = data->parsing.grid;
	i = 0;
	while (grid[i] && is_map_line(grid[i]))
		i++;
	while (grid[i])
	{
		if (is_map_line(grid[i]))
			clean_exit(data, "Map must not be seperated", 1);
		i++;
	}
	data->parsing.height = i;
}

int	check_map(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	find_elements(data);
	remove_elements(&data->parsing.grid, i);
	data->parsing.map = ft_strdup_double(data, data->parsing.grid);
	calculate_height(data);
	find_player(data, &j);
	if (!data->parsing.player_found)
		clean_exit(data, "Error: Player not found", 1);
	if (!flood_fill(data, data->player.x, data->player.y))
		clean_exit(data, "Error: Invalid map", 1);
	return (1);
}

void	parsing(t_data *data, char **argv, int argc)
{
	int		fd;

	fd = open(argv[1], O_RDONLY);
	if (fd == -1 || argc != 2)
		clean_exit(data, "Incorrect argument count", 1);
	init_map(data, argv[1]);
	data->parsing.map = ft_strdup_double(data, data->parsing.grid);
	data->parsing.elements_grid = ft_strdup_double(data, data->parsing.grid);
	check_map(data);
	map_width(data);
}
