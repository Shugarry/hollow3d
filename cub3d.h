/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 17:46:29 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/27 19:28:56 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

typedef struct s_texture	t_texture;

typedef struct s_player
{
	int		x;
	int		y;
	int		player_found;
}	t_player;
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
	t_texture	*texture;
	t_player	*player;
}						t_map;

typedef struct s_texture
{
	char	*n_tex;
	char	*s_tex;
	char	*e_tex;
	char	*w_tex;
	int		f_colour[4];
	int		c_colour[4];
	t_map	*map;
}					t_texture;

void	init_map_vars(t_map **map);
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
void	parse_texture(char *line, char **dest, char *id, t_map *map);
void	find_player(int *j, t_map *map);
void	error_and_free(char *str, t_map *map);
int		check_map(t_map *map);
int		flood_fill(t_map *map, int x, int y);
int		is_valid(char c);
int		parse_values(t_map *map, char *line);
int		is_map_line(char *line);
int		is_player(char c);
int		init_map(char *filename, t_map **map);
int		count_lines(char *filename);
char	**ft_split(char const *s, char c);
char	*get_next_line(int fd);
char	*ft_strdup(const char *s);
char	*ft_strchr(const char *s, int c);
char	*trim_line(char *line);
char	set_map_char(char **map, int x, int y, int height);
char	*trim_line(char *line);
char	**ft_strdup_double(char **str);
void print_grid(char **grid);
void	check_dup_element(t_map *map, char *line);
