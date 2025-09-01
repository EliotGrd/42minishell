/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 09:19:45 by bsuger            #+#    #+#             */
/*   Updated: 2025/05/02 17:53:26 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	in_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*result;
	char	*begin;
	char	*end;
	char	*save;

	begin = (char *) s1;
	end = (char *) s1;
	if (!s1)
		return (ft_strdup(""));
	while (*end)
		end++;
	while (in_set(*begin, set) == 1)
		begin += 1;
	if (begin > --end)
		return (ft_strdup(""));
	while (in_set(*end, set) == 1)
		end -= 1;
	result = malloc(sizeof(char) * (end - begin) + 2);
	if (result == NULL)
		return (NULL);
	save = result;
	while (begin <= end)
		*result++ = *begin++;
	*result = '\0';
	return (save);
}
