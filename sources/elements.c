/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elements.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:06:58 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/26 18:09:28 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libraries/libft/libft.h"

void	find_elements(int *i, t_map *map)
{
	char	*line;
	char	**grid;

	grid = map->grid;
	*i = 0;
	while (grid[*i] && !is_map_line(trim_line(grid[*i])))
	{
		line = trim_line(grid[*i]);
		if (*line == '\0')
		{
			(*i)++;
			continue ;
		}
		check_element(map, line);
		(*i)++;
	}
	check_if_found(map);
	init_values(map, *i);
}

void	remove_elements(t_map **map, int i)
{
	int	j;

	j = 0;
	while ((*map)->grid[i + j])
	{
		(*map)->grid[j] = (*map)->grid[i + j];
		j++;
	}
	(*map)->grid[j] = NULL;
}

void	check_if_found(t_map *map)
{
	if (map->ea_found == 0 || map->f_found == 0 || \
		map->no_found == 0 || map->so_found == 0 || \
		map->we_found == 0)
	{
		error_and_free("element missing", map);
	}
}

// TODO make init funtion and set colours to -1 to check for duplicates
void	init_values(t_map *map, int start)
{
	int		i;
	char	*line;

	i = start;
	while (map->grid[i])
	{
		line = trim_line(map->grid[i]);
		if (*line == '\0')
		{
			i++;
			continue ;
		}
		if (!parse_values(map, line))
			break ;
		i++;
	}
}

void	check_element(t_map *map, char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		map->no_found = 1;
	else if (ft_strncmp(line, "SO ", 3) == 0)
		map->so_found = 1;
	else if (ft_strncmp(line, "WE ", 3) == 0)
		map->we_found = 1;
	else if (ft_strncmp(line, "EA ", 3) == 0)
		map->ea_found = 1;
	else if (ft_strncmp(line, "F ", 2) == 0)
		map->f_found = 1;
	else if (ft_strncmp(line, "C ", 2) == 0)
		map->c_found = 1;
	else
		error_and_free("Invalid identifier", map);
}
