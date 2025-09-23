/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sprites.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:25:18 by joshapir          #+#    #+#             */
/*   Updated: 2025/03/04 18:27:13 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include "mlx_linux/include/MLX42/MLX42.h"
#include "MLX42_Int.h"

void	init_textures(t_game *game, mlx_image_t **img, char *path)
{
	mlx_texture_t	*texture;

	texture = mlx_load_png(path);
	if (!texture)
		return ;
	*img = mlx_texture_to_image(game->mlx, texture);
	mlx_delete_texture(texture);
}

void	init_sprites(t_game *game)
{
	init_textures(game, &game->img->wall, "./fences.png");
	init_textures(game, &game->img->floor, "./fl.png");
	init_textures(game, &game->img->collectibles, "./coin_1.png");
	init_textures(game, &game->img->exit, "./chest.png");
	init_textures(game, &game->img->player, "./t1.png");
	init_textures(game, &game->img->counter, "./counter.png");
	if (!game->img->wall || !game->img->floor || \
			!game->img->collectibles || !game->img->exit || !game->img->player)
		return ;
}
