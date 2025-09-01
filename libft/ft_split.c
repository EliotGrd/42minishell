/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 08:22:57 by bsuger            #+#    #+#             */
/*   Updated: 2025/06/24 13:52:01 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	i;
	int	words;

	i = 0;
	words = 0;
	if (s[0] == '\0')
		return (0);
	if (s[i] != c)
	{
		words++;
		i++;
	}
	while (s[i])
	{
		if (s[i] != c && s[i - 1] == c && s[i])
			words++;
		i++;
	}
	return (words);
}

static char	*fill(char const *s, char c, int j)
{
	int		k;
	char	*result;

	k = 0;
	while (s[j + k] != c && s[j + k])
		k++;
	result = malloc(sizeof(char) * k + 1);
	if (result == NULL)
		return (NULL);
	k = 0;
	while (s[j + k] != c && s[j + k])
	{
		result[k] = s[j + k];
		k++;
	}
	result[k] = '\0';
	return (result);
}

void	ft_free_all(char **result, int i)
{
	while (i--)
		free(result[i]);
	free(result);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	int		words;
	char	**result;

	i = 0;
	j = 0;
	words = count_words(s, c);
	result = malloc (sizeof(char *) * (words + 1));
	if (result == NULL)
		return (NULL);
	while (i < words)
	{
		while (s[j] == c && s[j])
			j++;
		result[i] = fill(s, c, j);
		if (result[i] == NULL)
			return (ft_free_all(result, i), NULL);
		j += ft_strlen(result[i]);
		i++;
	}
	result[i] = NULL;
	return (result);
}
