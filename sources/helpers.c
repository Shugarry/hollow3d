/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/26 18:06:50 by joshapir          #+#    #+#             */
/*   Updated: 2025/10/02 19:16:00 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int ft_isspace(int c)
{
	return (c == ' '  || c == '\t' || c == '\n'
			|| c == '\v' || c == '\f' || c == '\r');
}

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

void	print_grid(char **grid)
{
	int	i;

	i = 0;
	if (!grid)
		return ;
	while (grid[i])
	{
		printf("%s", grid[i]);
		i++;
	}
}

void	check_parsed_values(t_data *data)
{
	printf("1. map:\n");
	print_grid(data->parsing.map);
	printf("3. width %i\n", data->parsing.width);
	printf("4. height %i\n", data->parsing.height);
	printf("5. player_found %i\n", data->parsing.player_found);
	printf("6. no_found %i\n", data->parsing.no_found);
	printf("7. so_found %i\n", data->parsing.so_found);
	printf("8. ea_found %i\n", data->parsing.ea_found);
	printf("9. we_found %i\n", data->parsing.we_found);
	printf("10. f_found %i\n", data->parsing.f_found);
	printf("11. c_found %i\n", data->parsing.c_found);
	printf("12. n_tex%s\n", data->parsing.paths.n_tex);
	printf("13. s_tex%s\n", data->parsing.paths.s_tex);
	printf("14. e_tex%s\n", data->parsing.paths.e_tex);
	printf("15. w_tex%s\n", data->parsing.paths.w_tex);
	for (int i = 16; i < 4; i++)
		printf("17. %i %i\n", i, data->parsing.paths.f_colour[i]);
	for (int i = 18; i < 4; i++)
		printf("19. %i %i\n", i, data->parsing.paths.c_colour[i]);
	printf("20. %i\n", data->player.x);
	printf("21. %i\n", data->player.y);
	printf("22. player_dir = %c\n", data->player.direction);
}
