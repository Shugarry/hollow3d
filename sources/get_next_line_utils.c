/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 17:20:00 by joshapir          #+#    #+#             */
/*   Updated: 2024/11/09 17:21:06 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strchr(const char *s, int c)
{
	if (s == NULL)
		return (NULL);
	while (*s != '\0')
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (c == '\0')
		return ((char *)s);
	return (NULL);
}

char	*ft_strjoin(char *buffer, char *next) //works
{
	int		i;
	int		j;
	int		len;
	char	*new;

	i = 0;
	j = -1;
	if (next == NULL && buffer == NULL)
		return (NULL);
	len = (ft_strchr(next, '\0') - next) + (ft_strchr(buffer, '\0') - buffer);
	new = (char *)malloc(sizeof(char) * len + 1);
	if (!new)
		return (NULL);
	if (next != NULL)
	{
		while (next[i])
		{
			new[i] = next[i];
			i++;
		}
	}
	while (buffer[++j])
		new[i + j] = buffer[j];
	return (new[i + j] = '\0', new);
}
