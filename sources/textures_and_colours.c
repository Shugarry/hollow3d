/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_and_colours.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:08:09 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/29 18:14:02 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libraries/libft/libft.h"

int	parse_values(t_map *map, char *line)
{
	if (ft_strncmp(line, "NO ", 3) == 0)
		parse_texture(line + 3, &map->texture->n_tex, "NO", map);
	else if (ft_strncmp(line, "SO ", 3) == 0)
		parse_texture(line + 3, &map->texture->s_tex, "SO", map);
	else if (ft_strncmp(line, "WE ", 3) == 0)
		parse_texture(line + 3, &map->texture->w_tex, "WE", map);
	else if (ft_strncmp(line, "EA ", 3) == 0)
		parse_texture(line + 3, &map->texture->e_tex, "EA", map);
	else if (ft_strncmp(line, "F ", 2) == 0)
		parse_color(line + 2, map->texture->f_colour, "F", map);
	else if (ft_strncmp(line, "C ", 2) == 0)
		parse_color(line + 2, map->texture->c_colour, "C", map);
	else if (is_map_line(line))
		return (0);
	else
		error_and_free("Invalid identifier", map);
	return (1);
}

int	check_if_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			return (1);
		i++;
	}
	return (0);
}

void	check_values(char **split, t_map *map, int i)
{
	while (split[i])
	{
		if (!check_if_digit(split[i]))
		{
			free_double_array(split);
			errr_and_free("Error: RGB value is not a number", map);
		}
		i++;
	}
	if (i != 3 && i != 4)
		error_and_free("Invalid RGB format", map);
}

void	parse_color(char *line, int rgb[3], char *id, t_map *map)
{
	char	**split;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (*line == ' ')
		line++;
	if (rgb[0] != -1)
		error_and_free("Duplicate colour identifier", map);
	split = ft_split(line, ',');
	if (!split)
		error_and_free("Split Malloc failed", map);
	check_values(split, map, i);
	while (j < i)
	{
		rgb[j] = ft_atoi(split[j]);
		if (rgb[j] < 0 || rgb[j] > 255)
			error_and_free("RGB value out of range (0-255)", map);
		j++;
	}
	free_double_array(split);
}

void	parse_texture(char *line, char **dest, char *id, t_map *map)
{
	int	fd;

	while (*line == ' ')
		line++;
	if (*dest != NULL)
		error_and_free("Duplicate texture found", map);
	*dest = ft_strdup(line);
	if (!*dest)
		error_and_free("Malloc failed", map);
	fd = open(*dest, O_RDONLY);
	if (fd < 0)
		error_and_free("Texture file not found", map);
	close(fd);
}
