/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:07:34 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/30 17:34:47 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	init_map(t_data *data, char *filename)
{
	int	i;
	int	n_lines;
	int	fd;

	i = 0;
	n_lines = count_lines(filename);
	if (n_lines == -1)
		return (0);
	data->parsing.grid = (char **)memlist_alloc(data, \
											sizeof(char *) * (n_lines + 1));
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		clean_exit(data, "Error on opening file", 1);
	while (i < n_lines)
	{
		data->parsing.grid[i] = memlist_add(data, get_next_line(fd));
		i++;
	}
	data->parsing.grid[i] = NULL;
	close(fd);
	data->parsing.map = ft_strdup_double(data, data->parsing.grid);
	return (1);
}

void	init_values(t_data *data)
{
	int		i;
	char	*line;

	i = 0;
	while (data->parsing.grid[i])
	{
		line = trim_line(data->parsing.grid[i]);
		if (*line == '\0')
		{
			i++;
			continue ;
		}
		if (!parse_values(data, line))
			break ;
		i++;
	}
}
