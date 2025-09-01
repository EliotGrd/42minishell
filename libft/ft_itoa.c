/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 11:47:33 by bsuger            #+#    #+#             */
/*   Updated: 2025/05/03 13:12:07 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	number_digits(int n)
{
	size_t	digits;

	digits = 0;
	if (n == 0)
		return (1);
	while (n)
	{
		digits++;
		n /= 10;
	}
	return (digits);
}

char	*ft_itoa(int n)
{
	int				neg;
	int				digits;
	char			*result;
	long long int	temp;

	neg = 0;
	temp = n;
	if (n < 0)
	{
		neg = 1;
		temp = -temp;
	}
	digits = number_digits(temp);
	result = malloc(sizeof(char) * digits + neg + 1);
	if (result == NULL)
		return (NULL);
	if (neg)
		result[0] = '-';
	result[digits + neg] = '\0';
	while (--digits + neg >= neg)
	{
		result[digits + neg] = (temp % 10) + '0';
		temp /= 10;
	}
	return (result);
}
