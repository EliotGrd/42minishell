/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 08:22:26 by bsuger            #+#    #+#             */
/*   Updated: 2025/07/07 13:18:32 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf_char(char c)
{
	return (write(1, &c, 1));
}

int	ft_printf_str(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (i = write(1, "(null)", 6));
	while (s[i])
	{
		ft_printf_char(s[i]);
		i++;
	}
	return (i);
}

int	ft_convert(char format, va_list args)
{
	int	count;

	count = 0;
	if (format == 'c')
		count += ft_printf_char(va_arg(args, int));
	else if (format == 's')
		count += ft_printf_str(va_arg(args, char *));
	else if (format == 'p')
		count += ft_printf_ptr(va_arg(args, void *));
	else if (format == 'd' || format == 'i')
		count += ft_printf_numbers(va_arg(args, int));
	else if (format == 'u')
		count += ft_printf_unsigned_numbers(va_arg(args, unsigned int));
	else if (format == 'x' || format == 'X')
		count += ft_printf_hex(va_arg(args, unsigned int), format);
	else
		count += write(1, "%", 1);
	return (count);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	int		counter;
	va_list	args;

	i = 0;
	counter = 0;
	if (format == NULL)
		return (-1);
	va_start(args, format);
	while (format[i])
	{
		if (format[i] != '%')
			counter += write(1, &format[i], 1);
		else
			counter += ft_convert(format[++i], args);
		i++;
	}
	va_end(args);
	return (counter);
}
