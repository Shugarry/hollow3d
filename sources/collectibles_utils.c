/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collectibles_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 19:47:53 by joshapir          #+#    #+#             */
/*   Updated: 2025/03/04 21:49:14 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	collectibles(t_game *game, int x, int y)
{
	int				i;
	char			**grid;
	t_collectible	*c;

	i = 0;
	c = game-> c;
	grid = game->map->grid;
	if (grid[y][x] == 'C')
	{
		if (c->y != x || c-> x != y)
		{
			while (c && (c->y != x || c->x != y))
			{
				c = c-> next;
				i++;
			}
		}
		grid[y][x] = '0';
		game->collected++;
		game->img->collectibles->instances[i].enabled = false;
	}
}

void	collectible_count(t_game **game)
{
	int				count;
	int				i;
	t_collectible	*collectibles;

	i = 0;
	collectibles = (*game)->c;
	count = (*game)->total_collectibles;
	while (i < count - 1)
	{
		collectibles->next = malloc(sizeof(t_collectible));
		collectibles = collectibles-> next;
		i++;
	}
	collectibles->next = NULL;
}
