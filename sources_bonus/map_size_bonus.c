/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_size.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:28:52 by joshapir          #+#    #+#             */
/*   Updated: 2025/12/10 23:12:15 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	map_width(t_data *data)
{
	int	len;

	data->parsing.height = 0;
	data->parsing.width = 0;
	while (data->parsing.grid[data->parsing.height])
	{
		len = ft_strlen(data->parsing.grid[data->parsing.height]);
		if (len > data->parsing.width)
			data->parsing.width = len;
		data->parsing.height ++;
	}
}
