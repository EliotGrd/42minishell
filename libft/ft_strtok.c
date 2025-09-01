/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtok.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 09:37:55 by bsuger            #+#    #+#             */
/*   Updated: 2025/06/23 09:52:04 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in(char c, char *sep)
{
	while (*sep)
	{
		if (c == *sep)
			return (1);
		sep++;
	}
	return (0);
}

char	*ft_strtok(char *str, char *separators)
{
	char		*start;
	static char	*savepoint = NULL;

	if (!str)
		str = savepoint;
	if (!str)
		return (NULL);
	while (is_in(*str, separators) && *str)
		str++;
	if (!*str)
		return (savepoint = NULL, NULL);
	start = str;
	while (!is_in(*str, separators) && *str)
		str++;
	if (*str)
		savepoint = str + 1;
	else
		savepoint = NULL;
	*str = '\0';
	return (start);
}
