/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures_and_colours.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:08:09 by joshapir          #+#    #+#             */
/*   Updated: 2025/10/02 19:19:51 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	parse_values(t_data *data, char *line)
{
	if (ft_strncmp(line, "NO", 2) == 0 && ft_isspace(line[2]))
		parse_texture(data, line + 3, &data->parsing.paths.n_tex);
	else if (ft_strncmp(line, "SO", 2) == 0 && ft_isspace(line[2]))
		parse_texture(data, line + 3, &data->parsing.paths.s_tex);
	else if (ft_strncmp(line, "WE", 2) == 0 && ft_isspace(line[2]))
		parse_texture(data, line + 3, &data->parsing.paths.w_tex);
	else if (ft_strncmp(line, "EA", 2) == 0 && ft_isspace(line[2]))
		parse_texture(data, line + 3, &data->parsing.paths.e_tex);
	else if (ft_strncmp(line, "F", 1) == 0 && ft_isspace(line[1]))
		parse_color(data, line + 2, data->parsing.paths.f_color);
	else if (ft_strncmp(line, "C", 1) == 0 && ft_isspace(line[1]))
		parse_color(data, line + 2, data->parsing.paths.c_color);
	else if (is_map_line(line))
		return (0);
	else
		clean_exit(data, "Invalid identifier", 1);
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

void	check_values(t_data *data, char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		if (!check_if_digit(split[i]))
		{
			free_double_array(split);
			clean_exit(data, "Error: RGB value is not a number", 1);
		}
		i++;
	}
	if (i != 3 && i != 4)
		clean_exit(data, "Invalid RGB format", 1);
}

void	parse_color(t_data *data, char *line, int rgb[3])
{
	char	**split;
	int		j;

	j = 0;
	while (ft_isspace(*line))
		line++;
	if (rgb[0] != -1)
		clean_exit(data, "Duplicate colour identifier", 1);
	split = ft_split(line, ',');
	if (!split)
		clean_exit(data, "Split malloc failure", 1);
	check_values(data, split);
	while (split[j])
	{
		rgb[j] = ft_atoi(split[j]);
		if (rgb[j] < 0 || rgb[j] > 255)
			clean_exit(data, "RGB value out of range (0-255)", 1);
		j++;
	}
	free_double_array(split);
}

void	parse_texture(t_data *data, char *line, char **dest)
{
	int	fd;

	while (ft_isspace(*line))
		line++;
	if (*dest != NULL)
		clean_exit(data, "Duplicate texture found", 1);
	*dest = memlist_add(data, ft_strdup(line));
	fd = open(*dest, O_RDONLY);
	if (fd < 0)
		clean_exit(data, "Texture file not found", 1);
	close(fd);
}
