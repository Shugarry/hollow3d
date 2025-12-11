/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:32:00 by frey-gal          #+#    #+#             */
/*   Updated: 2025/12/11 21:48:43 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# include <stdint.h>
# include <string.h>
# include <sys/time.h>

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define WIN_WIDTH 960
# define WIN_HEIGHT 540
# define MOVE_SPEED 0.1
# define ROTATE_SPEED 0.06
# define SIXTY_FPS 0.0167
# define THIRTY_FPS 0.0334
# define MINI_SIZE 100
# define MINI_TILE_SIZE 8
# define MINI_RADIUS 80

# define PLAYER_COLOR 0xFFFFFFFF
# define PLAYER_DIR_COLOR 0xFF00FFFF
# define PLAYER_SIZE 4
# define MINIMAP_RADIUS 80
# define MINIMAP_VIEW_RADIUS 8
# define MINIMAP_CENTER_X MINIMAP_RADIUS
# define MINIMAP_CENTER_Y MINIMAP_RADIUS

typedef struct s_player
{
	int		x;
	int		y;
	double	curr_x;
	double	curr_y;
	char	direction;
	int		dead;
}	t_player;

typedef struct s_paths
{
	char	*n_tex;
	char	*s_tex;
	char	*e_tex;
	char	*w_tex;
	int		f_color[4];
	int		c_color[4];
}	t_paths;

typedef struct s_parsing
{
	char		**grid;
	char		**elements_grid;
	char		**map;
	int			width;
	int			height;
	int			player_found;
	int			no_found;
	int			so_found;
	int			ea_found;
	int			we_found;
	int			f_found;
	int			c_found;
	t_paths		paths;
}	t_parsing;

typedef struct s_textures
{
	mlx_texture_t	*ceiling;
	mlx_texture_t	*floor;
	mlx_texture_t	*north;
	mlx_texture_t	*south;
	mlx_texture_t	*east;
	mlx_texture_t	*west;
	mlx_texture_t	*fps_ui;
	uint32_t		floor_color;
	uint32_t		ceiling_color;
	int				tex_x;
	int				tex_y;
}	t_textures;

typedef struct s_raycast
{
	double	time;
	double	old_time;
	double	frame_time;
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
	double	wall_x;
	int		tex_x;
	int		tex_y;
	double	tex_step;
	double	tex_pos;
	double	ray_dir_x_left;
	double	ray_dir_x_right;
	double	ray_dir_y_left;
	double	ray_dir_y_right;
	int		pos_y;
	double	pos_z;
	double	row_distance;
	double	floor_step_x;
	double	floor_step_y;
	double	floor_x;
	double	floor_y;
	int		cell_x;
	int		cell_y;
	int		f_tex_x;
	int		f_tex_y;
}	t_raycast;

typedef struct s_animation
{
	bool			in_animation;
	int				frame_num;
	mlx_texture_t	*sword[5];
	int				sword_killed;
}	t_animation;

typedef struct s_doors
{
	bool			open;
	int				frame_num;
	int				draw_start;
	int				draw_end;
	int				line_height;
	bool			door_found;
	bool			in_animation;
	double			ray_distance;
	mlx_texture_t	*texture[7];

	double			door_x;
}	t_doors;

typedef struct s_enemy
{
	double			x;
	double			y;
	double			distance;
	mlx_texture_t	*texture;
	bool			alive;
	int				in_range;
}	t_enemy;

typedef struct s_circle
{
	int			center_x;
	int			center_y;
	int			radius;
	uint32_t	color;
}	t_circle;

typedef struct t_mini
{
	int		px;
	int		py;
	int		radius;
	int		player_tile_x;
	int		player_tile_y;
	int		tile_x;
	int		tile_y;
	int		dx;
	int		x1;
	int		y1;
	int		dy;
	int		steps;
	int		angle;
	int		dir_x;
	int		dir_y;
	int		screen_x;
	int		screen_y;
	float	x_inc;
	float	y_inc;

}	t_mini;

typedef struct s_enemy_vars
{
	double			dx;
	double			dy;
	double			move_speed;
	double			new_x;
	double			new_y;
	int				can_move;
	int				closest_enemy;
	double			closest_distance;
	double			res;
	double			transform_x;
	double			transform_y;
	int				draw_start_x;
	int				draw_end_x;
	int				stripe;
}	t_enemy_vars;

typedef struct s_line
{
	int		dx;
	int		dy;
	int		steps;
	float	x_inc;
	float	y_inc;
	float	x;
	float	y;
	int		i;
}	t_line;

typedef struct s_data
{
	t_player		player;
	t_textures		textures;
	t_raycast		raycast;
	t_parsing		parsing;
	t_animation		animation;
	t_doors			doors;
	double			scale;
	char			**map;
	mlx_t			*mlx;
	mlx_image_t		*canvas;
	mlx_image_t		*fps_str;
	mlx_image_t		*mini;
	t_list			*memlist;
	t_enemy			*enemies;
	t_enemy_vars	enemy_vars;
	t_mini			mini_m;
	t_circle		circle;
	t_line			line;
	int				enemy_count;
	int				sword_hit;
	double			*wall_distances;
	int				s_animation;
}	t_data;

typedef struct s_sprite_data
{
	int		sprite_screen_x;
	int		sprite_width;
	int		sprite_height;
	int		draw_start_y;
	int		draw_end_y;
	double	transform_y;
}	t_sprite_data;

//memory.c
void			*memlist_alloc(t_data *data, size_t size);
void			*memlist_add(t_data *data, void *ptr);
void			memlist_free_ptr(t_data *data, void *ptr);
void			clean_exit(t_data *data, char *error_str, int error_num);

// helpers.c
double			get_time_seconds(void);
uint32_t		rgba(int r, int g, int b, int a);
void			print_grid(char **grid);
void			free_double_array(char **arr);
int				ft_isspace(int c);
void			check_parsed_values(t_data *data);

// elements.c
void			find_elements(t_data *data);
void			check_element(t_data *data, char *line);
void			check_if_found(t_data *data);
void			remove_elements(char ***grid, int i);

// init_utils.c
void			init_values(t_data *data);
int				init_map(t_data *data, char *filename);

// map_utils.c
int				count_lines(char *filename);
char			*trim_line(char *line);
bool			is_player(char c);
void			find_player(t_data *data, int *j);
char			**ft_strdup_double(t_data *data, char **str);

// textures_and_colours.c
int				parse_values(t_data *data, char *line);
int				check_if_digit(char *str);
void			check_values(t_data *data, char **split);
void			parse_color(t_data *data, char *line, int rgb[3]);
void			parse_texture(t_data *data, char *line, char **dest);

// flood_fill.c
bool			is_valid(char c);
char			set_map_char(char **map, int x, int y, int height);
int				flood_fill(t_data *data, int x, int y);

// parsing.c
void			calculate_height(t_data *data);
int				check_map(t_data *data);
bool			is_map_line(char *line);
void			check_dup_element(t_data *data, char *line);
void			parsing(t_data *data, char **argv, int argc);

// movement.c
void			movement(t_data *data);

// camera.c
void			camera(t_data *data);

// raycasting.c
void			draw_floor_ceiling(t_data *data);
uint32_t		get_tex_pixel(mlx_texture_t *texture, int x, int y, int darken);
void			starting_vars(t_data *data);
void			draw_walls(t_data *data, int x);
void			raycaster(t_data *data);
void			init_mini(t_data *data);
void			update_minimap(t_data *data);
void			map_width(t_data *data);

// fps.c
void			img_to_window_scaled(t_data *data, mlx_texture_t *texture, \
				int pos_x, int pos_y);
void			fps_counter(t_data *data);

// sword_animation.c
void			sword_animation(t_data *data);

// enemies.c
void			init_enemies(t_data *data);
void			update_enemies(t_data *data);
void			sort_enemies(t_data *data);
void			draw_enemy(t_data *data, t_enemy *enemy);
uint32_t		get_tex_pixel(mlx_texture_t *texture, int x, int y, int darken);
void			draw_floor_ceiling(t_data *data);
void			draw_enemies_on_minimap(t_data *data);
void			check_if_dead(t_data *data);
void			check_if_dead(t_data *data);
void			calculate_enemy_distance(t_data *data, \
				int i, t_enemy_vars *vars);
void			check_if_can_move(t_data *data, int i, t_enemy_vars *vars);
void			update_enemy_pos(t_data *data, int i, t_enemy_vars *vars);
void			update_enemies(t_data *data);
void			sort_enemies(t_data *data);
void			get_sprite_transform(t_data *data, t_enemy *enemy, \
				double *transform_x, double *transform_y);
void			get_sprite_dimensions(double transform_x, \
				double transform_y, int *screen_x, int *height);
void			draw_enemy(t_data *data, t_enemy *enemy);
void			get_draw_bounds(int sprite_screen_x, int sprite_size, \
				int *start_x, int *end_x);
void			get_draw_bounds_y(int sprite_size, int *start_y, int *end_y);
void			draw_sprite_pixel(t_data *data, t_enemy *enemy, \
				int stripe, int y);
void			draw_sprite_column(t_data *data, t_enemy *enemy, int stripe, \
				t_sprite_data *sp);

//minimap.c
void			draw_enemies_on_minimap(t_data *data);
void			draw_minimap_background(t_data *data);
void			draw_minimap_circle_border(t_data *data);
void			draw_player_on_minimap(t_data *data);
void			draw_minimap_player(t_data *data);
void			update_enemy_pos(t_data *data, int i, t_enemy_vars *vars);
void			update_enemies(t_data *data);
void			sort_enemies(t_data *data);
void			get_sprite_transform(t_data *data, t_enemy *enemy, \
				double *transform_x, double *transform_y);
void			get_sprite_dimensions(double transform_x, \
				double transform_y, int *screen_x, int *height);
void			draw_minimap_circle(mlx_image_t *img, t_data *data, \
				int radius, uint32_t color);
void			draw_minimap_layout(t_data *data);
void			draw_tile_on_minimap(t_data *data, int x, \
				int y, uint32_t colour);
void			draw_square(mlx_image_t *img, int start_x, \
				int start_y, int size);
int				is_in_circle(int x, int y, int radius);
void			draw_line(mlx_image_t *img, t_data *data, int x1, int y1);

void			draw_door(t_data *data, int x);
mlx_texture_t	*draw_frames_opening(t_data *data, int frame);
mlx_texture_t	*draw_frames_closing(t_data *data, int frame);
void			get_door_texture_helper(t_doors *doors);
void			texturize_doors_helper(t_data *data, t_raycast *r, t_doors *d);
void			get_door_x(t_data *data);
void			check_door(t_data *data);
void			enemy_loop(t_data *data);

#endif
