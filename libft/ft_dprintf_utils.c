/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 12:07:36 by bsuger            #+#    #+#             */
/*   Updated: 2025/07/07 13:18:49 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_dprintf_numbers(int fd, int n)
{
	int	count;

	count = 0;
	if (n == -2147483648)
		return (write(fd, "-2147483648", 11));
	if (n < 0)
	{
		count += write(fd, "-", 1);
		n *= -1;
	}
	if (n >= 0 && n <= 9)
		count += ft_dprintf_char(fd, n + '0');
	if (n > 9)
	{
		count += ft_dprintf_numbers(fd, n / 10);
		count += ft_dprintf_numbers(fd, n % 10);
	}
	return (count);
}

int	ft_dprintf_unsigned_numbers(int fd, unsigned int n)
{
	int	count;

	count = 0;
	if (n <= 9)
		count += ft_dprintf_char(fd, n + '0');
	if (n > 9)
	{
		count += ft_dprintf_unsigned_numbers(fd, n / 10);
		count += ft_dprintf_unsigned_numbers(fd, n % 10);
	}
	return (count);
}

int	ft_dprintf_hex(int fd, unsigned int n, char format)
{
	int	count;

	count = 0;
	if (n <= 15)
	{
		if (format == 'x')
			count += ft_dprintf_char(fd, "0123456789abcdef"[n]);
		else
			count += ft_dprintf_char(fd, "0123456789ABCDEF"[n]);
	}
	if (n > 15)
	{
		count += ft_dprintf_hex(fd, n / 16, format);
		count += ft_dprintf_hex(fd, n % 16, format);
	}
	return (count);
}

int	ft_dprintf_address(int fd, unsigned long long n)
{
	int	count;

	count = 0;
	if (n <= 15)
		count += ft_dprintf_char(fd, "0123456789abcdef"[n]);
	if (n > 15)
	{
		count += ft_dprintf_address(fd, n / 16);
		count += ft_dprintf_address(fd, n % 16);
	}
	return (count);
}

int	ft_dprintf_ptr(int fd, void *ptr)
{
	int					count;
	unsigned long long	n;

	if (ptr == NULL)
		return (write(fd, "(nil)", 5));
	n = (unsigned long long) ptr;
	count = write(fd, "0x", 2);
	count += ft_dprintf_address(fd, n);
	return (count);
}
