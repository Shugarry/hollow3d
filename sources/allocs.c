/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:54:34 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/23 18:31:34 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	allocs(t_game **game, t_map **map)
{
	(*map) = malloc(sizeof(t_map));
	if (!(*map))
		return (0);
	(*game) = (t_game *)malloc(sizeof(t_game));
	if (!(*game))
		return (0);
	(*game)-> map = *map;
	(*game)->img = malloc(sizeof(t_img));
	if (!(*game)->img)
		return (0);
	(*game)->c = malloc(sizeof(t_collectible));
	if (!(*game)-> c)
		return (0);
	return (1);
}
