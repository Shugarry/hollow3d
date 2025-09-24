# include <stdlib.h>
# include <unistd.h>
#include <fcntl.h>

typedef struct s_texture	t_texture;
typedef struct s_map
{
	char		**grid;
	char		**grid2;
	int			width;
	int			height;
	int			exit_found;
	int			player_found;
	int			NO_found;
	int			SO_found;
	int			EA_found;
	int			WE_found;
	int			F_found;
	t_texture	*texture;
}	t_map;

typedef struct s_texture
{
    char *n_tex;
    char *s_tex;
    char *e_tex;
    char *w_tex;
    int f_colour[4];
    int c_colour[4];
    t_map *map;
}	t_texture;
