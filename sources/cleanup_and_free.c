/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_and_free.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:06:50 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/30 17:33:57 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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

void	cleanup(t_parsing *map)
{
	t_texture	tex;

	tex = map->texture;
	if (map->grid)
		free_double_array(map->grid);
	if (map->elements_grid)
		free_double_array(map->elements_grid);
	if (map->map)
		free_double_array(map->map);
	if (tex.e_tex)
		free(tex.e_tex);
	if (tex.n_tex)
		free(tex.n_tex);
	if (tex.s_tex)
		free(tex.s_tex);
	if (tex.w_tex)
		free(tex.w_tex);
	//free(map);
}

//TODO add free for e_tex (caused seg)
void	error_and_free(char *str, t_parsing *map)
{
	printf("%s\n", str);
	if (map)
		cleanup(map);
	exit(0);
}
