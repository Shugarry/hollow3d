/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 20:10:01 by joshapir          #+#    #+#             */
/*   Updated: 2025/09/25 16:23:29 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdio.h>
#include "cube3d.h"

char	**ft_len_assign(char **arr, char const *s, char c, int count)
{
	int	i;
	int	j;
	int	len;
	int	start;

	j = 0;
	i = 0;
	while (s[i] && count > 0)
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		len = (i - start);
		arr[j] = (char *)malloc(sizeof(char) * len + 1);
		if (!arr[j])
			return (NULL);
		j++;
		count--;
	}
	return (arr);
}

int	ft_word_count(char const *s, char c)
{
	int	count;
	int	i;

	i = 0;
	count = 0;
	while (s[i] == c)
		i++;
	while (s[i] != '\0')
	{
		while (s[i] && (i == 0 || s[i - 1] == c))
		{
			count++;
			i++;
		}
		while (s[i] != c && s[i] != '\0')
			i++;
		while (s[i] == c && s[i])
			i++;
	}
	return (count);
}

char	**ft_split(char const *s, char c)
{
	int		count;
	char	**arr;
	int		i;
	int		current_word;
	int		j;

	i = 0;
	count = ft_word_count(s, c);
	arr = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	arr[count] = NULL;
	current_word = 0;
	ft_len_assign(arr, s, c, count);
	while (s[i] == c)
		i++;
	while (current_word < count)
	{
		while (s[i] != c && s[i] != '\0')
		{
			arr[current_word][j] = s[i];
			j++;
			i++;
		}
		arr[current_word][j] = '\0';
		current_word++;
		j = 0;
		while (s[i] == c)
			i++;
	}
	return (arr);
}
// int main (void)
// {
//     char **result;
//     int i;

//     // Test case 1: Splitting a sentence into words
//     // if i < len - c
//     result = ft_split("   Life    is   good   asd         ", ' ');
//     if (result)
//     {
//         printf("Test case 1: Splitting \"Life is good\" by ' '\n");
//         for (i = 0; result[i] != NULL; i++)
//         {
//             printf("result[%d]: %s\n", i, result[i]);
//             free(result[i]); // Free each word after use
//         }
//             printf("result[%d]: %s\n", i, result[i]);

//         free(result); // Free the array of pointers
//     }
// }
