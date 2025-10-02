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

int	ft_dprintf_char(int fd, char c)
{
	return (write(fd, &c, 1));
}

int	ft_dprintf_str(int fd, char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (i = write(fd, "(null)", 6));
	while (s[i])
	{
		ft_dprintf_char(fd, s[i]);
		i++;
	}
	return (i);
}

int	ft_dconvert(int fd, char format, va_list args)
{
	int	count;

	count = 0;
	if (format == 'c')
		count += ft_dprintf_char(fd, va_arg(args, int));
	else if (format == 's')
		count += ft_dprintf_str(fd ,va_arg(args, char *));
	else if (format == 'p')
		count += ft_dprintf_ptr(fd ,va_arg(args, void *));
	else if (format == 'd' || format == 'i')
		count += ft_dprintf_numbers(fd, va_arg(args, int));
	else if (format == 'u')
		count += ft_dprintf_unsigned_numbers(fd, va_arg(args, unsigned int));
	else if (format == 'x' || format == 'X')
		count += ft_dprintf_hex(fd, va_arg(args, unsigned int), format);
	else
		count += write(fd, "%", 1);
	return (count);
}

int	ft_dprintf(int fd, const char *format, ...)
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
			counter += write(fd, &format[i], 1);
		else
			counter += ft_dconvert(fd, format[++i], args);
		i++;
	}
	va_end(args);
	return (counter);
}
