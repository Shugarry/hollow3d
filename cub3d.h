/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 21:14:17 by joshapir          #+#    #+#             */
/*   Updated: 2025/03/04 20:56:03 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "libraries/libft/libft.h"
# include "libraries/libft/ft_printf.h"
# include "libraries/libft/get_next_line.h"
# include "libraries/MLX42/include/MLX42/MLX42.h"
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <stdbool.h>
# include <stdio.h>

typedef struct s_data
{
	t_list	*memlist;
}	t_data;

//memory.c - Better memory management functions
void	*memlist_alloc(t_data *data, size_t size);
void	*memlist_add(t_data *data, void *ptr);
void	memlist_free_ptr(t_data *data, void *ptr);
void	clean_exit(t_data *data, char *error_str, int error_num);

#endif
