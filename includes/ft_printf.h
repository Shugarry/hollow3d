/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joshapir <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 22:26:36 by joshapir          #+#    #+#             */
/*   Updated: 2024/10/19 20:26:43 by joshapir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>

int		ft_putstr(char *s);
int		ft_if(const char *str, int i, va_list args, int count);
int		ft_printf(const char *str, ...);
void	ft_putnbr_hex(unsigned long n, int uppercase, int first, int *count);
void	ft_putnbr(unsigned int n, int sign, int *count);
#endif
