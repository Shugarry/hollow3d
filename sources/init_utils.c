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

#include "cub3d.h"
#include "libraries/libft/libft.h"

int	init_map(char *filename, t_map *map)
{
	int	i;
	int	line_count;
	int	fd;

	i = 0;
	line_count = count_lines(filename);
	if (line_count == -1)
		return (0);
	map->grid = (char **)malloc(sizeof(char *) * (line_count + 1));
	if (!map->grid)
		return (0);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (cleanup(map), 0);
	while (i < line_count)
	{
		map->grid[i] = get_next_line(fd);
		if (!map->grid[i])
			error_and_free("error with gnl", map);
		i++;
	}
	map->grid[i] = NULL;
	close(fd);
	map->map = ft_strdup_double(map->grid);
	return (1);
}

void	init_map_vars(t_map *map)
{
	int			i;
	t_texture	tex;

	i = 0;
	//*map = malloc(sizeof(t_map));
	//(*map)->texture = malloc(sizeof(t_texture));
//	(*map)->player = malloc(sizeof(t_player));
	// if (!(*map)->texture)
	// 	error_and_free("struct malloc failed", *map);
	tex = map->texture;
	map->ea_found = 0;
	map->f_found = 0;
	map->no_found = 0;
	map->so_found = 0;
	while (i < 3)
	{
		tex.c_colour[i] = -1;
		tex.f_colour[i] = -1;
		i++;
	}
	tex.e_tex = NULL;
	tex.n_tex = NULL;
	tex.s_tex = NULL;
	tex.w_tex = NULL;
	map->texture = tex;
	map->player_found = 0;
}

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
