/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 18:22:46 by joshapir          #+#    #+#             */
/*   Updated: 2024/10/19 20:19:30 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include <limits.h>
//#include <stdarg.h>
//#include <unistd.h>
#include "ft_printf.h"

void	ft_putnbr_hex(unsigned long n, int uppercase, int first, int *count)
{
	char	*digits;

	if (!n && first)
	{
		*count += write(1, "(nil)", 5);
		return ;
	}
	if (first)
		*count += write(1, "0x", 2);
	if (uppercase)
		digits = "0123456789ABCDEF";
	else
		digits = "0123456789abcdef";
	if (n > 15)
		ft_putnbr_hex(n / 16, uppercase, 0, count);
	*count += write(1, &digits[n % 16], 1);
}

void	ft_putnbr(unsigned int n, int sign, int *count)
{
	int		new;
	char	c;

	if (sign)
	{
		new = (int)n;
		if (new < 0)
		{
			*count += write (1, "-", 1);
			new = -new;
			n = new;
		}
	}
	if (n > 9)
		ft_putnbr(n / 10, 1, count);
	c = n % 10 + '0';
	*count += write(1, &c, 1);
}

int	ft_putstr(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
	{
		write(1, "(null)", 6);
		return (6);
	}
	while (s[i])
	{
		write(1, &s[i], 1);
		i ++;
	}
	return (i);
}

int	ft_if(const char *str, int i, va_list args, int count)
{
	char	c;

	if (str[i] == 'i' || str[i] == 'd')
		ft_putnbr(va_arg(args, int), 1, &count);
	if (str[i] == 'u')
		ft_putnbr(va_arg(args, unsigned int), 0, &count);
	if (str[i] == 'x')
		ft_putnbr_hex(va_arg(args, unsigned int), 0, 0, &count);
	if (str[i] == 'X')
		ft_putnbr_hex(va_arg(args, unsigned int), 1, 0, &count);
	if (str[i] == 'c')
	{
		c = ((char)va_arg(args, int));
		count += write (1, &c, 1);
	}
	if (str[i] == 's')
		count += ft_putstr(va_arg(args, char *));
	if (str[i] == 'p')
		ft_putnbr_hex(va_arg(args, unsigned long), 0, 1, &count);
	if (str[i] == '%')
		count += write(1, "%", 1);
	return (count);
}

int	ft_printf(const char *str, ...)
{
	int		count;
	int		i;
	va_list	args;

	va_start(args, str);
	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '%')
		{
			++i;
			count = ft_if(str, i, args, count);
		}
		else
			count += write (1, &str[i], 1);
		i++;
	}
	va_end(args);
	return (count);
}
/*
int main (void)
{
   int i;
   unsigned int j = -1234;
// unsigned int i = -1;

i = ft_printf("Mine: %d %d %d %d %d %d %d", INT_MAX, 
INT_MIN, LONG_MAX, LONG_MIN, ULONG_MAX, 0, -42);

printf("\nReal: %d %d %d %d %d %d %d", INT_MAX,
INT_MIN, LONG_MAX, LONG_MIN, ULONG_MAX, 0, -42);
//printf("\n%i %d\n", 1, 2);
}*/
