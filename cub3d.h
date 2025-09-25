# include <stdlib.h>
# include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

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
	int			C_found;
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

int	ft_atoi(const char *nptr);
char	**ft_split(char const *s, char c);
void error_and_free(char *str, t_map *map);
char	*get_next_line(int fd);
char	*ft_strdup(const char *s);
char	*ft_strchr(const char *s, int c);
size_t	ft_strlen(const char	*s);
//char	*ft_strjoin(char const *s1, char const *s2);
