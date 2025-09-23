/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:57:56 by joshapir          #+#    #+#             */
/*   Updated: 2025/03/04 21:47:45 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	update_moves(t_game *game)
{
	char	*move_text;

	move_text = ft_itoa(game->moves);
	mlx_image_to_window(game->mlx, game->img->counter, 0, 0);
	mlx_put_string(game->mlx, move_text, 10, 10);
	free (move_text);
}

void	print_moves(t_game *game)
{
	static int	prev;

	if (prev != game->moves)
		ft_printf("Moves = %d\n", game->moves);
	prev = game->moves;
}

void	moves(mlx_key_data_t keydata, void *param)
{
	t_game	*game;
	char	**grid;
	int		x;
	int		y;

	game = (t_game *) param;
	x = game->img->player->instances-> x / TILE_SIZE;
	y = game->img->player->instances-> y / TILE_SIZE;
	grid = game->map->grid;
	if ((keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_UP) \
			&& keydata.action == MLX_PRESS && grid[y - 1][x] != '1')
		move_up(&game);
	if ((keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_DOWN) \
			&& keydata.action == MLX_PRESS && grid[y + 1][x] != '1')
		move_down(&game);
	if ((keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT) \
			&& keydata.action == MLX_PRESS && grid[y][x - 1] != '1')
		move_left(&game);
	if ((keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_RIGHT) \
			&& keydata.action == MLX_PRESS && grid[y][x + 1] != '1')
		move_right(&game);
	if ((keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS))
		return (init_exit(game), exit(0));
	print_moves(game);
	return (update_moves(game), collectibles(game, x, y), init_exit(game));
}
