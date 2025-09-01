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

int	ft_printf_numbers(int n)
{
	int	count;

	count = 0;
	if (n == -2147483648)
		return (write(1, "-2147483648", 11));
	if (n < 0)
	{
		count += write(1, "-", 1);
		n *= -1;
	}
	if (n >= 0 && n <= 9)
		count += ft_printf_char(n + '0');
	if (n > 9)
	{
		count += ft_printf_numbers(n / 10);
		count += ft_printf_numbers(n % 10);
	}
	return (count);
}

int	ft_printf_unsigned_numbers(unsigned int n)
{
	int	count;

	count = 0;
	if (n <= 9)
		count += ft_printf_char(n + '0');
	if (n > 9)
	{
		count += ft_printf_unsigned_numbers(n / 10);
		count += ft_printf_unsigned_numbers(n % 10);
	}
	return (count);
}

int	ft_printf_hex(unsigned int n, char format)
{
	int	count;

	count = 0;
	if (n <= 15)
	{
		if (format == 'x')
			count += ft_printf_char("0123456789abcdef"[n]);
		else
			count += ft_printf_char("0123456789ABCDEF"[n]);
	}
	if (n > 15)
	{
		count += ft_printf_hex(n / 16, format);
		count += ft_printf_hex(n % 16, format);
	}
	return (count);
}

int	ft_printf_address(unsigned long long n)
{
	int	count;

	count = 0;
	if (n <= 15)
		count += ft_printf_char("0123456789abcdef"[n]);
	if (n > 15)
	{
		count += ft_printf_address(n / 16);
		count += ft_printf_address(n % 16);
	}
	return (count);
}

int	ft_printf_ptr(void *ptr)
{
	int					count;
	unsigned long long	n;

	if (ptr == NULL)
		return (write(1, "(nil)", 5));
	n = (unsigned long long) ptr;
	count = write(1, "0x", 2);
	count += ft_printf_address(n);
	return (count);
}
