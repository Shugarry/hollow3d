# include "cube3d.h"

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
		return (free_map(map), free((*map)-> grid), 0);
	while (i < line_count)
	{
		(*map)->grid[i] = get_next_line(fd);
		if (!(*map)->grid[i])
			return (free_map(map), free((*map)->grid), 0);
		i++;
	}
	(*map)->grid[i] = NULL;
	close(fd);
	return (1);
}

void	find_player(int *i, int *j, char **grid)
{
	while (grid[(*i)])
	{
		while (grid[(*i)][(*j)])
		{
			if (grid[(*i)][(*j)] == 'P')
				return ;
			(*j)++;
		}
		(*i)++;
		(*j) = 0;
	}
}


int is_map_line(char *line)
{
    int i = 0;
    while (line[i] == ' ')
        i++;
    if (line[i] == '\0')
        return 0;

    while (line[i])
    {
        if (line[i] != '0' && line[i] != '1' &&
            line[i] != 'N' && line[i] != 'S' &&
            line[i] != 'E' && line[i] != 'W' &&
            line[i] != ' ')
            return 0;
        i++;
    }
    return 1;
}

void	is_element()
{
	if (strncmp(grid[*i], "NO ", 3) == 0)
	else if (strncmp(grid[*i], "SO ", 3) == 0)
	else if (strncmp(grid[*i], "WE ", 3) == 0)
	else if (strncmp(grid[*i], "EA ", 3) == 0)
	else if (strncmp(grid[*i], "F ", 2) == 0)
	else if (strncmp(grid[*i], "C ", 2) == 0)
}

void parse_texture(char *line, char **dest, char *id)
{
    while (*line == ' ')
        line++;

    if (*dest != NULL)
        error("Duplicate texture found");
    *dest = ft_strdup(line);
    if (!*dest)
        error("Malloc failed");
    int fd = open(*dest, O_RDONLY);
    if (fd < 0)
        error("Texture file not found");
    close(fd);
}

void parse_color(char *line, int rgb[3], const char *id)
{
    char **split;
    int i;
	int j;

	i = 0;
	j = 0;
    while (*line == ' ')
        line++;
    if (rgb[0] != -1)
        error("Duplicate colour identifier");

    split = ft_split(line, ',');
    if (!split)
        error("Malloc failed for split");
    while (split[i])
		i++;
    if (i != 3 || i != 4)
        error("Invalid RGB format");
    while (j < i)
    {
        rgb[j] = ft_atoi(split[j]);
        if (rgb[j] < 0 || rgb[j] > 255)
            error("RGB value out of range (0-255)");
		j++;
    }
    free_split(split);
}
// TODO make init funtion and set colours to -1 to check for duplicates
void init_values(t_map *map)
{
    int i = 0;
    char *line;

    while (map->grid[i])
    {
        line = map->grid[i];
        while (*line == ' ')
            line++;
        if (strncmp(line, "NO ", 3) == 0)
            parse_texture(line + 3, &map->texture->n_tex, "NO");
        else if (strncmp(line, "SO ", 3) == 0)
            parse_texture(line + 3, &map->texture->s_tex, "SO");
        else if (strncmp(line, "WE ", 3) == 0)
            parse_texture(line + 3, &map->texture->w_tex, "WE");
        else if (strncmp(line, "EA ", 3) == 0)
            parse_texture(line + 3, &map->texture->e_tex, "EA");
        else if (strncmp(line, "F ", 2) == 0)
            parse_color(line + 2, map->texture->f_colour, "F");
        else if (strncmp(line, "C ", 2) == 0)
            parse_color(line + 2, map->texture->c_colour, "C");
        else if (is_map_line(line))
            break;
        else if (*line != '\0')
            error("Invalid identifier");
        i++;
    }
}

// 6 identifiers (NO, SO, WE, EA, F, C).
void find_elements(int *i, t_map *map)
{
    char **grid = map->grid;

    while (grid[*i] && !is_map_line(grid[*i]))
    {
        if (strncmp(grid[*i], "NO ", 3) == 0)
            map->NO_found = 1;
        else if (strncmp(grid[*i], "SO ", 3) == 0)
            map->SO_found = 1;
        else if (strncmp(grid[*i], "WE ", 3) == 0)
            map->WE_found = 1;
        else if (strncmp(grid[*i], "EA ", 3) == 0)
            map->EA_found = 1;
        else if (strncmp(grid[*i], "F ", 2) == 0)
            map->F_found = 1;
        else if (strncmp(grid[*i], "C ", 2) == 0)
            map->C_found = 1;
        else if (strlen(grid[*i]) > 0)
            error("Invalid identifier");
        (*i)++;
    }
	//check_if_found

	//init_values();
}


int	valid_path(t_map *map)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	map->exit_found = 0;
	find_elements(&i, map);
	find_player(&i, &j, map-> grid);
	flood_fill(map, map->grid, j, i);
	if (!map->exit_found)
		return (ft_printf("error\nexit not found\n"), 0);
	if (!map->player_found)
		return (ft_printf("Error\nPlayer not found\n"), 0);
	return (1);
}

void	calculate_dimensions(t_map *map)
{
	char	**grid;
	int		i;
	int		j;

	i = 0;
	j = 0;
	grid = map->grid;
	map->width = ft_strlen(map->grid[0]);
	while (grid[i])
	{
		j = 0;
		while (grid[i][j])
		{
			j++;
		}
		i++;
	}
	map->height = i;
}
int	validate_map(t_map **map, t_game **game, char *argv)
{
	if (!init_map(argv, map))
		return (free_grid((*map)->grid), 0);
	calculate_dimensions(*map);
	if (!is_rectangle(*map))
		return (0);
	if (!check_walls(*map))
		return (0);
	if (!valid_path(*map))
		return (0);
	free_grid((*map)->grid);
	free((*map)->grid);
	if (!init_map(argv, map))
		return (0);
	if (!init_components(*map, *game))
		return (0);
	return (1);
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
