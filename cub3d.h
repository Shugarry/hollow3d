#ifndef CUB3D_H
# define CUB3D_H

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

# define WIN_WIDTH 1280
# define WIN_HEIGHT 720

// NOTE: FIX STRUCTS

typedef struct s_texture	t_texture;

typedef struct s_player
{
	int		player_found;
	int		x;
	int		y;
	double	curr_x;
	double	curr_y;
	char	direction;
}	t_player;

typedef struct s_texture
{
	char	*n_tex;
	char	*s_tex;
	char	*e_tex;
	char	*w_tex;
	int		f_colour[4];
	int		c_colour[4];
}	t_texture;

typedef struct s_map
{
	char		**grid;
	char		**elements_grid;
	char		**map;
	int			width;
	int			height;
	int			exit_found;
	int			player_found;
	int			no_found;
	int			so_found;
	int			ea_found;
	int			we_found;
	int			f_found;
	int			c_found;
	t_texture	texture;
	t_player	player;
}						t_map;

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

typedef struct s_raycast
{
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	int		map_x;
	int		map_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	int		step_x;
	int		step_y;
	bool	hit;
	int		side;
	int		line_height;
	int		draw_start;
	int		draw_end;
}	t_raycast;

typedef struct s_data
{
	t_player	player;
	t_paths		paths;
	t_textures	textures;
	t_images	images;
	t_raycast	raycast;
	t_map		parsing;
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

//joes functions
void	init_map_vars(t_map *map);
void	error_and_free(char *str, t_map *map);
void	cleanup(t_map *map);
void	free_double_array(char **arr);
void	remove_elements(char ***grid, int i);
void	calculate_height(t_map *map, int start);
void	find_elements(t_map *map);
void	check_element(t_map *map, char *line);
void	check_if_found(t_map *map);
void	init_values(t_map *map, int start);
void	parse_color(char *line, int rgb[3], char *id, t_map *map);
void	parse_texture(char *line, char *dest, char *id, t_map *map);
void	find_player(int *j, t_map *map);
void	error_and_free(char *str, t_map *map);
int		check_map(t_map *map);
int		flood_fill(t_map *map, int x, int y);
int		is_valid(char c);
int		parse_values(t_map *map, char *line);
int		is_map_line(char *line);
int		is_player(char c);
int		init_map(char *filename, t_map *map);
int		count_lines(char *filename);
char	*trim_line(char *line);
char	set_map_char(char **map, int x, int y, int height);
char	*trim_line(char *line);
char	**ft_strdup_double(char **str);
void	print_grid(char **grid);
void	check_dup_element(t_map *map, char *line);
void	check_values(char **split, t_map *map, int i);

#endif
