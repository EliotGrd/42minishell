/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 15:24:53 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/03 16:31:24 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (0);
	if (!s1 || !*s1 || !s2 || !*s2)
		return (0);
	while (s1[i] && i < n - 1 && s1[i] == s2[i])
	{
		i++;
	}
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}
