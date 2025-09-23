/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 20:39:28 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/23 18:33:02 by frey-gal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	init_window(t_game *game)
{
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	game->mlx = mlx_init((game->map->width - 1) * TILE_SIZE, \
	game->map->height * TILE_SIZE, "so_long", true);
	game->win = mlx_new_image(game->mlx, game->map->width, game->map->height);
	mlx_image_to_window(game->mlx, game->win, 0, 0);
}

char	*ft_strcpy(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s2[i] != '\0')
	{
		s1[i] = s2[i];
		i++;
	}
	s1[i] = '\0';
	return (s1);
}

int	main(int argc, char **argv)
{
	t_map	*map;
	t_game	*game;
	int		fd;

	(void) map;
	fd = open(argv[1], O_RDONLY);
	if (fd == -1 || argc != 2)
		return (ft_printf("ERROR\n"), 0);
	allocs(&game, &map);
	game->collected = 0;
	game->moves = 0;
	if (!validate_map(&map, &game, argv[1]))
		return (cleanup(&game), 0);
	collectible_count(&game);
	init_window(game);
	init_sprites(game);
	render_map(game);
	ft_printf("Moves = 0\n");
	mlx_key_hook(game->mlx, moves, game);
	mlx_loop(game->mlx);
	free_all(game, map);
	return (0);
}
