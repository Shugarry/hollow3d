/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elements.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:06:58 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/11 21:58:30 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	find_elements(t_data *data)
{
	char	*line;
	char	**grid;
	int		i;

	grid = data->parsing.elements_grid;
	i = 0;
	while (grid[i] && !is_map_line(trim_line(grid[i])))
	{
		line = trim_line(grid[i]);
		if (*line == '\0')
		{
			i++;
			continue ;
		}
		check_element(data, line);
		i++;
	}
	check_if_found(data);
	init_values(data);
}

void	remove_elements(char ***grid, int i)
{
	int	j;

	j = 0;
	i = 0;
	while ((*grid)[i] && !is_map_line((*grid)[i]))
		i++;
	while ((*grid)[i + j])
	{
		(*grid)[j] = (*grid)[i + j];
		j++;
	}
	(*grid)[j] = NULL;
}

void	check_if_found(t_data *data)
{
	if (data->parsing.ea_found == 0 || data->parsing.f_found == 0 || \
		data->parsing.no_found == 0 || data->parsing.so_found == 0 || \
		data->parsing.we_found == 0)
	{
		clean_exit(data, "Element missing", 1);
	}
}

void	check_element(t_data *data, char *line)
{
	check_dup_element(data, line);
	if (ft_strncmp(line, "NO", 2) == 0 && ft_isspace(line[2]))
		data->parsing.no_found = 1;
	else if (ft_strncmp(line, "SO", 2) == 0 && ft_isspace(line[2]))
		data->parsing.so_found = 1;
	else if (ft_strncmp(line, "WE", 2) == 0 && ft_isspace(line[2]))
		data->parsing.we_found = 1;
	else if (ft_strncmp(line, "EA", 2) == 0 && ft_isspace(line[2]))
		data->parsing.ea_found = 1;
	else if (ft_strncmp(line, "F", 1) == 0 && ft_isspace(line[1]))
		data->parsing.f_found = 1;
	else if (ft_strncmp(line, "C", 1) == 0 && ft_isspace(line[1]))
		data->parsing.c_found = 1;
	else
		clean_exit(data, "Invalid Identifier", 1);
}

void	check_dup_element(t_data *data, char *line)
{
	if ((ft_strncmp(line, "NO", 2) == 0 && data->parsing.no_found) \
		|| (ft_strncmp(line, "SO", 2) == 0 && data->parsing.so_found) \
		|| (ft_strncmp(line, "WE", 2) == 0 && data->parsing.we_found) \
		|| (ft_strncmp(line, "EA", 2) == 0 && data->parsing.ea_found) \
		|| (ft_strncmp(line, "F", 1) == 0 && data->parsing.f_found) \
		|| (ft_strncmp(line, "C", 1) == 0 && data->parsing.c_found))
		clean_exit(data, "Error, duplicate element found", 1);
}
