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
# include <math.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/stat.h>
# include <fcntl.h>

# define PI 3.14159265358979323846
# define WIN_WIDTH 1280
# define WIN_HEIGHT 720
# define NORTH 90
# define SOUTH 270
# define EAST 0
# define WEST 180 

typedef struct s_player
{
	int		start_x;
	int		start_y;
	double	curr_x;
	double	curr_y;
	char	direction;
}	t_player;

typedef struct s_paths
{
	char	*ceiling;
	char	*floor;
	char	*north;
	char	*south;
	char	*east;
	char	*west;
}	t_paths;

typedef struct s_textures
{
	mlx_texture_t	*ceiling;
	mlx_texture_t	*floor;
	mlx_texture_t	*north;
	mlx_texture_t	*south;
	mlx_texture_t	*east;
	mlx_texture_t	*west;
}	t_textures;

typedef struct s_images
{
	mlx_image_t	*ceiling;
	mlx_image_t	*floor;
	mlx_image_t	*north;
	mlx_image_t	*south;
	mlx_image_t	*east;
	mlx_image_t	*west;
}	t_images;

typedef struct s_data
{
	t_player	player;
	t_paths		paths;
	t_textures	textures;
	t_images	images;
	char		**map;
	mlx_t		*mlx;
	mlx_image_t	*canvas;

	t_list		*memlist;
}	t_data;

//memory.c - Better memory management functions
void	*memlist_alloc(t_data *data, size_t size);
void	*memlist_add(t_data *data, void *ptr);
void	memlist_free_ptr(t_data *data, void *ptr);
void	clean_exit(t_data *data, char *error_str, int error_num);

#endif
