/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_free_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:31:18 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/23 18:33:02 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	free_grid(char **grid)
{
	int	i;

	if (!grid)
		return ;
	if (grid)
	{
		i = 0;
		while (grid[i])
		{
			free(grid[i]);
			i++;
		}
	}
	grid = NULL;
	if (grid)
		ft_printf("grid isnt null = %s\n", grid[0]);
}

void	free_map(t_map **map_struct)
{
	int	i;

	if (!(*map_struct) || !(*map_struct) || !(*map_struct)-> grid)
		return ;
	if ((*map_struct)->grid)
	{
		i = 0;
		while ((*map_struct)->grid[i])
		{
			free((*map_struct)->grid[i]);
			i++;
		}
	}
	if ((*map_struct)->grid)
		free((*map_struct)->grid);
	if (*map_struct)
		free(*map_struct);
	*map_struct = NULL;
}
