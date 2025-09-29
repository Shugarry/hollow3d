/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_and_free.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:06:50 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/29 19:05:16 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include "libraries/libft/libft.h"

void	free_double_array(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	cleanup(t_map *map)
{
	int			i;
	t_texture	*tex;

	tex = map->texture;
	if (map->grid)
		free_double_array(map->grid);
	if (map->elements_grid)
		free_double_array(map->elements_grid);
	if (map->map)
		free_double_array(map->map);
	if (tex->e_tex)
		free(tex->e_tex);
	if (tex->n_tex)
		free(tex->n_tex);
	if (tex->s_tex)
		free(tex->s_tex);
	if (tex)
		free(tex);
	if (map->player)
		free(map->player);
	free(map);
}

//TODO add free for e_tex (caused seg)
void	error_and_free(char *str, t_map *map)
{
	printf("%s\n", str);
	if (map)
		cleanup(map);
	exit(0);
}
