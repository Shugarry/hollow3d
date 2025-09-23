/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:39:17 by joshapir          #+#    #+#             */
/*   Updated: 2024/11/09 17:42:05 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1
# endif

# ifndef OPEN_MAX
#  define OPEN_MAX 1024
# endif

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>

char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char *buffer, char *next);
char	*fill_next(int fd, char *next, ssize_t bytes_read);
char	*construct_line(char *next);
char	*clean_next(char *next);
char	*get_next_line(int fd);

#endif
