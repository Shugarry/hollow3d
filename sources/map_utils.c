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

int	is_player(char c)
{
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

void	find_player(int *j, t_map *map)
{
	char	**grid;
	int		i;

	i = 0;
	grid = map->grid;
	while (grid[i])
	{
		while (grid[i][(*j)])
		{
			if (is_player(grid[i][(*j)]))
			{
				if (map->player_found)
					error_and_free("Error: Multiple players found", map);
				printf("player_found\n");
				map->player_found = 1;
				map->player.x = i;
				map->player.y = *j;
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

char	**ft_strdup_double(char **str) // NOTE: not safe
{
	char	**dup;
	int		i;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i])
		i++;
	dup = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (str[i])
	{
		dup[i] = ft_strdup(str[i]);
		i++;
	}
	dup[i] = NULL;
	return (dup);
}
