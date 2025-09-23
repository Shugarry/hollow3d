/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:36:33 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/23 18:33:02 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	move_right(t_game **game)
{
	(*game)->img->player->instances[0].x += TILE_SIZE;
	(*game)->moves++;
}

void	move_left(t_game **game)
{
	(*game)->img->player->instances[0].x -= TILE_SIZE;
	(*game)->moves++;
}

void	move_down(t_game **game)
{
	(*game)->img->player->instances[0].y += TILE_SIZE;
	(*game)->moves++;
}

void	move_up(t_game **game)
{
	(*game)->img->player->instances[0].y -= TILE_SIZE;
	(*game)->moves++;
}
