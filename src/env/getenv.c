/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 08:15:26 by bsuger            #+#    #+#             */
/*   Updated: 2025/08/22 13:14:19 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_path(char *str, char *str2, char sep)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = malloc(sizeof(char) * (ft_strlen(str) + ft_strlen(str2) + 2));
	if (!result)
		return (ft_putendl_fd(MALLOC_ERR, 2), NULL);
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	result[i] = sep;
	i++;
	while (str2[j])
	{
		result[i + j] = str2[j];
		j++;
	}
	result[i + j] = '\0';
	return (result);
}

char	*search_path(char *str, t_env *top_env)
{
	int		i;
	char	*result;
	char	*path;
	char	**split_envp;

	i = 0;
	if (top_env == NULL)
		return (NULL);
	path = research_key_env(top_env, "PATH");
	if (!path)
		return (NULL);
	split_envp = ft_split(path, ':');
	if (!split_envp)
		return (ft_putendl_fd(MALLOC_ERR, 2), NULL);
	while (split_envp[i])
	{
		result = join_path(split_envp[i++], str, '/');
		if (!result)
			return (ft_free((void **)&path), ft_free_tab(split_envp), NULL);
		if (access(result, X_OK) == 0)
			return (ft_free((void **)&path), ft_free_tab(split_envp), result);
		ft_free((void **)&result);
	}
	return (ft_free((void **)&path), ft_free_tab(split_envp), NULL);
}
