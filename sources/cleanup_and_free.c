/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_and_free.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:06:50 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/26 18:06:52 by joshapir         ###   ########.fr       */
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
	free(tex->e_tex);
	free(tex->n_tex);
	free(tex->s_tex);
	free(tex->w_tex);
	free(tex);
	free(map);
}

void	error_and_free(char *str, t_map *map)
{
	printf("%s\n", str);
	cleanup(map);
	exit(0);
}
