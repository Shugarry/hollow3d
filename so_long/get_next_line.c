/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 16:53:49 by joshapir          #+#    #+#             */
/*   Updated: 2024/11/09 18:12:09 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*fill_next(int fd, char *next, ssize_t bytes_read)
{
	char	*buffer;
	char	*temp;

	bytes_read = 1;
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	while (bytes_read > 0 && !ft_strchr(next, '\n'))
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			temp = ft_strjoin(buffer, next);
			free (next);
			next = temp;
		}
		else if (bytes_read == 0)
			return (free (buffer), next);
		else
			return (free (buffer), NULL);
	}
	return (free (buffer), next);
}

char	*construct_line(char *next)
{
	int		line_len;
	int		i;
	char	*line;
	char	*nl_pos;

	i = 0;
	nl_pos = ft_strchr(next, '\n');
	if (nl_pos)
		line_len = nl_pos - next + 1;
	else
		line_len = ft_strchr(next, '\0') - next;
	line = (char *)malloc(sizeof(char) * (line_len + 1));
	if (!line)
		return (NULL);
	while (i < line_len)
	{
		line[i] = next[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

char	*clean_next(char *next) //works
{
	int		i;
	int		len_next;
	int		nl;
	char	*new;

	i = -1;
	len_next = ft_strchr(next, '\0') - next;
	if (ft_strchr(next, '\n'))
		nl = ft_strchr(next, '\n') - next;
	else
		return (NULL);
	if (next[nl] == '\n')
		nl++;
	new = (char *)malloc(sizeof(char) * (len_next - nl + 1));
	if (!new)
		return (NULL);
	while (next[nl + ++i])
		new[i] = next[nl + i];
	if (len_next == nl)
		return (free(new), NULL);
	else
		return (new[i] = '\0', new);
}

char	*get_next_line(int fd)
{
	static char	*next = NULL;
	char		*line;
	char		*temp;
	ssize_t		bytes_read;

	bytes_read = 1;
	if (fd <= -1 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX || fd >= OPEN_MAX)
		return (NULL);
	if (!next || !ft_strchr(next, '\n'))
		next = fill_next(fd, next, bytes_read);
	if (!next)
		return (NULL);
	line = construct_line(next);
	if (!line)
	{
		free(next);
		next = NULL;
		return (NULL);
	}
	temp = clean_next(next);
	free(next);
	next = temp;
	return (line);
}
/*
int main(int argc, char **argv)
{
int fd;
char *line;

if (argc != 2)
{
fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
return 1;
}

fd = open(argv[1], O_RDWR);
if (fd == -1)
{
perror("Error opening file");
return 1;
}

while ((line = get_next_line(fd)) != NULL)
{
printf("%s", line);
free(line);
}

close(fd);
return 0;
}
*/
