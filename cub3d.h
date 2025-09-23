/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:14:17 by joshapir          #+#    #+#             */
/*   Updated: 2025/03/04 20:56:03 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "libraries/libft/libft.h"
# include "libraries/libft/ft_printf.h"
# include "libraries/libft/get_next_line.h"
# include "libraries/MLX42/include/MLX42/MLX42.h"
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>

# define TILE_SIZE 32

typedef struct s_player
{
	int		x;
	int		y;
	int		collect_counter;
	int		moves;
}	t_player;
/*
typedef struct s_txt
{
	mlx_texture_t	*wall;
	mlx_texture_t	*path;
	mlx_texture_t	*E;
	mlx_texture_t	*C;
	mlx_texture_t	*P;
}   t_sprite;
*/
typedef struct s_collectible
{
	int						x;
	int						y;
	struct s_collectible	*next;
}	t_collectible;
typedef struct s_img
{
	mlx_image_t	*floor;
	mlx_image_t	*wall;
	mlx_image_t	*collectibles;
	mlx_image_t	*exit;
	mlx_image_t	*player;
	mlx_image_t	*counter;
}	t_img;
//mlx_texture_to_image

//if (x, y) == '1'
//   mlx_image_to_window(wall);

typedef struct s_map
{
	char	**grid;
	char	**grid2;
	int		width;
	int		height;
	int		exit_found;
	int		player_found;
}	t_map;

typedef struct s_win
{
	mlx_t		*mlx;
	t_player	player;
	t_map		map;
	t_img		*img;
}	t_win;

typedef struct s_textures
{
	mlx_texture_t	*floor;
	mlx_texture_t	*wall;
	mlx_texture_t	*collectibles;
	mlx_texture_t	*exit;
	mlx_texture_t	*player;
	mlx_texture_t	*counter;
}	t_textures;

typedef struct s_game
{
	mlx_t			*mlx;
	mlx_image_t		*win;
	void			*wall;
	void			*floor;
	void			*player;
	void			*exit;
	void			*collectible;
	t_img			*img;
	t_map			*map;
	int				player_x;
	int				player_y;
	int				moves;
	int				collected;
	int				total_collectibles;
	t_collectible	*c;
}	t_game;
void	free_map(t_map **node);
void	free_grid(char **grid);
void	find_player(int *i, int *j, char **grid);
void	init_player_pos(t_game *game);
void	collectibles(t_game *game, int x, int y);
void	init_exit(t_game *game);
void	update_moves(t_game *game);
void	cleanup(t_game **game);
void	move_up(t_game **game);
void	move_down(t_game **game);
void	move_left(t_game **game);
void	move_right(t_game **game);
void	free_collectibles(t_collectible **collectibles);
void	check_components(t_map *map, int *exit, int *collectables, int *s_pos);
void	find_player(int *i, int *j, char **grid);
void	calculate_dimensions(t_map *map);
void	free_grid(char **grid);
void	free_map(t_map **map_struct);
void	map_grid(char **grid);
void	init_window(t_game *game);
void	init_textures(t_game *game, mlx_image_t **img, char *path);
void	init_sprites(t_game *game);
void	assign_axis(t_game *game, int x, int y);
void	init_render(t_game *game, int *x, int *y, t_collectible **c);
void	render_map(t_game *game);
void	update_moves(t_game *game);
void	update_moves(t_game *game);
void	moves(mlx_key_data_t keydata, void *param);
void	init_player_pos(t_game *game);
void	init_exit(t_game *game);
void	collectibles(t_game *game, int x, int y);
void	collectible_count(t_game **game);
void	close_game(void *param);
void	free_all(t_game *game, t_map *map);
int		init_components(t_map *map, t_game *game);
int		init_map(char *filename, t_map **map);
int		is_rectangle(t_map *map);
int		count_lines(char *filename);
int		validate_map(t_map **map, t_game **game, char *argv);
int		allocs(t_game **game, t_map **map);
int		valid_component(t_map *map, int x, int y);
bool	is_valid_path(char **grid, int x, int y, int *collectibles);
#endif
