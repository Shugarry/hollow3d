/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <joshapir@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 18:24:13 by joshapir          #+#    #+#             */
/*   Updated: 2025/03/04 18:24:40 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

int	ft_size(long n)
{
	int	counter;

	counter = 0;
	if (n < 0)
	{
		n = n * -1;
		counter = 1;
	}
	if (n > 0)
	{
		while (n > 9)
		{
			n = n / 10;
			counter ++;
		}
		counter ++;
	}
	if (n == 0)
		counter = 1;
	return (counter);
}

char	*loop(int size, int k, long n)
{
	char	buffer;
	char	*arr;

	arr = malloc((size + 1));
	if (!arr)
		return (NULL);
	arr[size] = '\0';
	if (n < 0)
	{
		n = -n;
		arr[0] = '-';
		k = 1;
	}
	if (n == 0)
		arr[0] = '0';
	while (size > k)
	{
		buffer = (n % 10) + '0';
		arr[size - 1] = buffer;
		size --;
		n = n / 10;
	}
	return (arr);
}

char	*ft_itoa(int n)
{
	int		size;
	int		k;

	n = (long) n;
	k = 0;
	size = ft_size(n);
	return (loop(size, k, n));
}
