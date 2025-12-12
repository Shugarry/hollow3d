/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:07:45 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/30 17:02:37 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	count_lines(char *filename)
{
	int		lines;
	char	*line;
	int		fd;

	fd = open(filename, O_RDONLY);
	lines = 0;
	if (fd == -1)
		return (-1);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		lines++;
		free(line);
	}
	close(fd);
	return (lines);
}

bool	is_player(char c)
{
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (true);
	return (false);
}

void	find_player(t_data *data, int *j)
{
	char	**grid;
	int		i;

	i = 0;
	grid = data->parsing.grid;
	while (grid[i])
	{
		while (grid[i][(*j)])
		{
			if (is_player(grid[i][(*j)]))
			{
				if (data->parsing.player_found)
					clean_exit(data, "Error: Multiple players found", 1);
				data->parsing.player_found = 1;
				data->player.x = i;
				data->player.y = *j;
				data->player.direction = grid[i][(*j)];
			}
			(*j)++;
		}
		i++;
		(*j) = 0;
	}
}

char	*trim_line(char *line)
{
	int	i;
	int	j;
	int	len;
	int	k;

	i = 0;
	k = 0;
	len = ft_strlen(line);
	while (line[i] == ' ')
		i++;
	j = len - 1;
	while (j >= i && (line[j] == ' ' || line[j] == '\n' || line[j] == '\r'))
		j--;
	while (i <= j)
		line[k++] = line[i++];
	line[k] = '\0';
	return (line);
}

char	**ft_strdup_double(t_data *data, char **str)
{
	char	**dup;
	int		i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
		i++;
	dup = memlist_alloc(data, sizeof(char *) * (i + 1));
	i = 0;
	while (str[i])
	{
		dup[i] = memlist_add(data, ft_strdup(str[i]));
		i++;
	}
	dup[i] = NULL;
	return (dup);
}
