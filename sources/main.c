# include "cub3d.h"
# include "libraries/libft/libft.h"

#include <stdlib.h>




void free_double_array(char **arr)
{
    int i;

	i = 0;
    if (!arr)
        return;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void cleanup(t_map *map)
{
	int i;
	t_texture *tex;

	tex = map->texture;
	free(tex->e_tex);
	free(tex->n_tex);
	free(tex->s_tex);
	free(tex->w_tex);
	free(tex);
	free(map);
}

void error_and_free(char *str, t_map *map)
{
	printf("%s\n", str);
	//cleanup(map);
	exit(0);
}
void	init_map_vars(t_map **map)
{
	int i;

	i = 0;
	t_texture *tex;
	*map = malloc(sizeof(t_map));
	(*map)->texture = malloc(sizeof(t_texture));
	(*map)->player = malloc(sizeof(t_player));
	if (!(*map)->texture)
		error_and_free("struct malloc failed", *map);
	tex = (*map)->texture;
	(*map)->EA_found = 0;
	(*map)->F_found = 0;
	(*map)->NO_found = 0;
	(*map)->SO_found = 0;
	while(i < 3)
	{
		tex->c_colour[i] = -1;
		tex->f_colour[i] = -1;
		i++;
	}
	tex->e_tex = NULL;
	tex->n_tex = NULL;
	tex->s_tex = NULL;
	tex->w_tex = NULL;
	tex->map = *map;

}

int	count_lines(char *filename)
{
	int		lines;
	char	*line;
	int		fd;

	fd = open(filename, O_RDONLY);
	lines = 0;
	if (fd == -1)
		return (-1);
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		lines++;
		free(line);
	}
	close(fd);
	return (lines);
}

int	init_map(char *filename, t_map **map)
{
	int	i;
	int	line_count;
	int	fd;

	i = 0;
	line_count = count_lines(filename);
	if (line_count == -1)
		return (0);
	(*map)->grid = (char **)malloc(sizeof(char *) * (line_count + 1));
	if (!(*map)->grid)
		return (0);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (cleanup(*map), 0);
	while (i < line_count)
	{

		(*map)->grid[i] = get_next_line(fd);
		if (!(*map)->grid[i])
			return (cleanup(*map), 0);
		i++;
	}
	(*map)->grid[i] = NULL;
	close(fd);
	//printf("map line 1 = %s\n", (*map)->grid[0]);
	return (1);
}
int	is_player(char c)
{
	if (c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

void	find_player(int *i, int *j, t_map *map)
{
	char **grid;

	*i = 0;
	//printf("line = %s\n", map->grid[0]);
	grid = map->grid;
	printf("i = %d\n", *i);
	while (grid[(*i)])
	{
		while (grid[(*i)][(*j)])
		{
			if (is_player(grid[(*i)][(*j)]))
			{
				printf("player_found\n");
				map->player_found = 1;
				map->player->x = *i;
				map->player->y = *j;
				return ;
			}
			(*j)++;
		}
		(*i)++;
		(*j) = 0;
	}
}


int is_map_line(char *line)
{
	int i;

	i = 0;
    while (line[i] == ' ')
        i++;
    if (line[i] == '\0')
        return 0;

    while (line[i])
    {
		//printf("i in is_map_line = %d\n", i);
        if (line[i] != '0' && line[i] != '1' &&
            line[i] != 'N' && line[i] != 'S' &&
            line[i] != 'E' && line[i] != 'W' &&
            line[i] != ' ')
            return (0);
        i++;
    }
    return (1);
}

// void	is_element(char *grid, int *i)
// {
// 	if (strncmp(grid[*i], "NO ", 3) == 0)
// 	else if (strncmp(grid[*i], "SO ", 3) == 0)
// 	else if (strncmp(grid[*i], "WE ", 3) == 0)
// 	else if (strncmp(grid[*i], "EA ", 3) == 0)
// 	else if (strncmp(grid[*i], "F ", 2) == 0)
// 	else if (strncmp(grid[*i], "C ", 2) == 0)
// }

void parse_texture(char *line, char **dest, char *id, t_map *map)
{
    while (*line == ' ')
        line++;

    if (*dest != NULL)
        error_and_free("Duplicate texture found", map);
    *dest = ft_strdup(line);
    if (!*dest)
        error_and_free("Malloc failed", map);
    int fd = open(*dest, O_RDONLY);
    if (fd < 0)
        error_and_free("Texture file not found", map);
    close(fd);
}

void parse_color(char *line, int rgb[3], char *id, t_map *map)
{
    char **split;
    int i;
	int j;

	i = 0;
	j = 0;
    while (*line == ' ')
        line++;
    if (rgb[0] != -1)
        error_and_free("Duplicate colour identifier", map);

    split = ft_split(line, ',');
    if (!split)
        error_and_free("Split Malloc failed", map);
    while (split[i])
		i++;
    if (i != 3 && i != 4)
        error_and_free("Invalid RGB format", map);
    while (j < i)
    {
        rgb[j] = ft_atoi(split[j]);
        if (rgb[j] < 0 || rgb[j] > 255)
            error_and_free("RGB value out of range (0-255)", map);
		j++;
    }
    free_double_array(split);
}
// TODO make init funtion and set colours to -1 to check for duplicates
void init_values(t_map *map, int start)
{
    int i;
    char *line;

	i = start;
    while (map->grid[i])
    {
        line = trim_line(map->grid[i]);
        if (*line == '\0')
        {
            i++;
            continue;
        }

        if (strncmp(line, "NO ", 3) == 0)
            parse_texture(line + 3, &map->texture->n_tex, "NO", map);
        else if (strncmp(line, "SO ", 3) == 0)
            parse_texture(line + 3, &map->texture->s_tex, "SO", map);
        else if (strncmp(line, "WE ", 3) == 0)
            parse_texture(line + 3, &map->texture->w_tex, "WE", map);
        else if (strncmp(line, "EA ", 3) == 0)
            parse_texture(line + 3, &map->texture->e_tex, "EA", map);
        else if (strncmp(line, "F ", 2) == 0)
            parse_color(line + 2, map->texture->f_colour, "F", map);
        else if (strncmp(line, "C ", 2) == 0)
            parse_color(line + 2, map->texture->c_colour, "C", map);
        else if (is_map_line(line))
            break;
        else
            error_and_free("Invalid identifier", map);
        i++;
    }
}

void	check_if_found(t_map *map)
{
	if (map->EA_found == 0 || map->F_found == 0 || map->NO_found == 0 || map->SO_found == 0 || map->WE_found == 0)
	{
		printf ("EA = %d, F = %d, NO = %d, SO = %d, WE = %d\n", map->EA_found, map->F_found, map->NO_found, map->SO_found, map->WE_found);
		error_and_free("element missing", map);
	}
}

char *trim_line(char *line)
{
    int i;
    int j;
    int len;
 	int k;

	i = 0;
	k = 0;
	len = strlen(line);
    while (line[i] == ' ')
        i++;
    j = len - 1;
    while (j >= i && (line[j] == ' ' || line[j] == '\n' || line[j] == '\r'))
        j--;
    while (i <= j)
        line[k++] = line[i++];
    line[k] = '\0';
    return (line);
}



// 6 identifiers (NO, SO, WE, EA, F, C).

void find_elements(int *i, t_map *map)
{
    char *line;
    char **grid = map->grid;

    *i = 0;
    while (grid[*i] && !is_map_line(trim_line(grid[*i])))
    {
        line = trim_line(grid[*i]);

        if (*line == '\0')
        {
            (*i)++;
            continue;
        }

        if (strncmp(line, "NO ", 3) == 0)
            map->NO_found = 1;
        else if (strncmp(line, "SO ", 3) == 0)
            map->SO_found = 1;
        else if (strncmp(line, "WE ", 3) == 0)
            map->WE_found = 1;
        else if (strncmp(line, "EA ", 3) == 0)
            map->EA_found = 1;
        else if (strncmp(line, "F ", 2) == 0)
            map->F_found = 1;
        else if (strncmp(line, "C ", 2) == 0)
            map->C_found = 1;
        else
            error_and_free("Invalid identifier", map);

        (*i)++;
    }
    printf("i at end of find_elements = %d\n", *i);
    check_if_found(map);
    init_values(map, *i);
}


// void find_elements(int *i, t_map *map)
// {
//     char **grid = map->grid;
// 	*i = 0;
//     while (grid[(*i)] && !is_map_line(grid[(*i)]))
//     {
//         if (strncmp(grid[*i], "NO ", 3) == 0)
//             map->NO_found = 1;
//         else if (strncmp(grid[*i], "SO ", 3) == 0)
//             map->SO_found = 1;
//         else if (strncmp(grid[*i], "WE ", 3) == 0)
//             map->WE_found = 1;
//         else if (strncmp(grid[*i], "EA ", 3) == 0)
//             map->EA_found = 1;
//         else if (strncmp(grid[*i], "F ", 2) == 0)
//             map->F_found = 1;
//         else if (strncmp(grid[*i], "C ", 2) == 0)
//             map->C_found = 1;
//         else if (strlen(grid[*i]) > 0)
//             error_and_free("Invalid identifier", map);
//         (*i)++;
// 	}
// 	check_if_found(map);
// 	init_values(map);
// }
void	calculate_height(t_map *map, int start)
{
    int i;
	char **grid;

	grid = map->grid;
	i = 0;
	start = 0;
	printf("start = %d\n", start);
	printf("line[i] = %s\n", grid[start]);
    while (grid[i] && is_map_line(grid[i]))
    {
        i++;
    }
    while (grid[i])
    {
        if (is_map_line(grid[i]))
        {
            error_and_free("Map must not be seperated", map);
        }
        i++;
    }
    map->height = i;
}

void	remove_elements(t_map **map, int i)
{
	int j;

	j = 0;
	while ((*map)->grid[i + j])
	{
    	(*map)->grid[j] = (*map)->grid[i + j];
    	j++;
	}
	(*map)->grid[j] = NULL;
}
// 1, 0 space, v, N, S, E, W
int	is_valid(char c)
{
	if (c == '1' || c == '0' || c == ' ' || c == 'v' || c == 'N' || c == 'S' || c == 'E' || c == 'W')
		return (1);
	return (0);
}

char set_map_char(char **map, int x, int y, int height)
{
	int len;

    if (x < 0 || x >= height)
        return ' ';
    len = strlen(map[x]);
    if (x < 0 || x >= len)
        return ' ';
    return map[x][y];
}

int flood_fill(t_map *map, int x, int y)
{
    char c;

	// printf(" player x = %d player y = %d\n", x, y);
	// printf("height = %d\n", map->height);
	// c = set_map_char(map->grid, x, y, map->height);
    // if (c == ' ')
	// {
	// 	printf("here\n");
    //     return 0;
	// }
    // if (c == '1' || c == 'v')
    //     return 1;
    // if (!is_valid(c))
	// {
    //     return 0;
	// }
    // c = map->grid[x][y];

	// char c;

    c = set_map_char(map->grid, x, y, map->height);

    if (c == ' ')
	{
		printf("here\n");
        return 0;
	}
    if (c == '1' || c == 'v')
        return 1;
    if (!is_valid(c))
        return (0);
	 map->grid[x][y] = 'v';
    if (!flood_fill(map, x + 1, y))
	{

		return 0;
	}
    if (!flood_fill(map, x - 1, y))
	{

		return 0;
	}
    if (!flood_fill(map, x, y + 1))
	{
		return (0);
	}
    if (!flood_fill(map, x, y - 1))
	{
		printf("here\n");
		return (0);
	}

    return (1);
}


int	valid_path(t_map *map)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	//map->exit_found = 0;
	find_elements(&i, map);
	remove_elements(&map, i);
	calculate_height(map, i);

	find_player(&i, &j, map);

	if (!map->player_found)
		error_and_free("Player not found", map);
	if (!flood_fill(map, map->player->x, map->player->y))
		error_and_free("Map_invalid", map);

	// if (!map->exit_found)
	// 	return (ft_printf("error\nexit not found\n"), 0);
	// if (!map->player_found)
	// 	return (ft_printf("Error\nPlayer not found\n"), 0);
	return (1);
}

// void	calculate_dimensions(t_map *map)
// {
// 	char	**grid;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	j = 0;
// 	grid = map->grid;
// 	map->width = ft_strlen(map->grid[0]);
// 	while (grid[i])
// 	{
// 		j = 0;
// 		while (grid[i][j])
// 		{
// 			j++;
// 		}
// 		i++;
// 	}
// 	map->height = i;
// }

// int	validate_map(t_map **map, t_game **game, char *argv)
// {
// 	// if (!init_map(argv, map))
// 	// 	return (free_grid((*map)->grid), 0);
// 	calculate_dimensions(*map);
// 	if (!is_rectangle(*map))
// 		return (0);
// 	if (!check_walls(*map))
// 		return (0);
// 	if (!valid_path(*map))
// 		return (0);
// 	free_grid((*map)->grid);
// 	free((*map)->grid);
// 	if (!init_components(*map, *game))
// 		return (0);
// 	return (1);
// }


int	main(int argc, char **argv)
{
	t_map	*map;
	int		fd;

	(void) map;
	fd = open(argv[1], O_RDONLY);
	if (fd == -1 || argc != 2)
		return (printf("ERROR\n"), 0);
	//allocs(&game, &map);
	init_map_vars(&map);
	init_map(argv[1], &map);
	valid_path(map);
	// game->collected = 0;
	// game->moves = 0;
	// if (!validate_map(&map, argv[1]))
	// 	return (cleanup(&game), 0);
	// collectible_count(&game);
	// init_window(game);
	// init_sprites(game);
	// render_map(game);
	// ft_printf("Moves = 0\n");
	// mlx_key_hook(game->mlx, moves, game);
	// mlx_loop(game->mlx);
	// free_all(game, map);
	return (0);
}
