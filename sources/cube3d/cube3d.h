# include <stdlib.h>
# inlcude <unistd.h>


typedef struct s_map
{
	char	**grid;
	char	**grid2;
	int		width;
	int		height;
	int		exit_found;
	int		player_found;
	int		NO_found;
	int		SO_found;
	int		EA_found;
	int		WE_found;
	int		F_found;
}	t_map;

typedef struct s_config
{
    char *n_tex;
    char *s_tex;
    char *e_tex;
    char *w_tex;
    int floor_colour[3];
    int ceiling_colour[3];
    t_map *map;
}	t_config;
