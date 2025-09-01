/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:25:51 by bsuger            #+#    #+#             */
/*   Updated: 2025/07/19 15:02:24 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(char *str, size_t n)
{
	size_t	i;
	char	*result;

	i = 0;
	if (!str)
		return (NULL);
	if (ft_strlen(str) < n)
		n = ft_strlen(str);
	result = malloc(sizeof(char) * n + 1);
	if (!result)
		return (NULL);
	while (i < n)
	{
		result[i] = str[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
