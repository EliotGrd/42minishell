/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env_two.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:22:48 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/22 09:48:41 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*research_node_env(t_env *top_env, char *search)
{
	while (top_env)
	{
		if (ft_strncmp(search, top_env -> key, ft_strlen(search) + 1) == 0)
			return (top_env);
		top_env = top_env -> next;
	}
	return (NULL);
}

char	**split_env(char *envp)
{
	int		cursor;
	char	*temp;
	char	**result;

	cursor = 0;
	temp = envp;
	result = malloc(sizeof(char *) * 2);
	if (!result)
		return (ft_putendl_fd(MALLOC_ERR, 2), NULL);
	while (*temp && *temp != '=')
	{
		temp++;
		cursor++;
	}
	result[0] = ft_strndup(envp, cursor);
	if (!result[0])
		return (ft_putendl_fd(MALLOC_ERR, 2), free(result), NULL);
	result[1] = ft_strdup(envp + cursor + 1);
	if (!result[1])
		return (ft_putendl_fd(MALLOC_ERR, 2), free(result[0]), free(result), NULL);
	return (result);
}

char	*research_key_env(t_env *top_env, char *search)
{
	if (ft_strlen(search) == 0)
		return (NULL);
	while (top_env)
	{
		if (ft_strncmp(search, top_env -> key, ft_strlen(search) + 1) == 0)
			return (ft_strdup(top_env -> value));
		top_env = top_env -> next;
	}
	return (NULL);
}

char	**lst_to_tab_env(t_env *top)
{
	int		i;
	int		size;
	char	**result;

	i = 0;
	if (top == NULL)
		return (result = NULL);
	size = env_lst_size(top);
	result = malloc(sizeof(char *) * (size + 1));
	if (!result)
		return (NULL);
	while (i < size)
	{
		result[i] = join_path(top -> key, top -> value, '=');
		top = top -> next;
		i++;
	}
	result[i] = NULL;
	return (result);
}

int	set_up_env(t_env **top_env, char **envp)
{
	int		i;
	char	**temp;
	t_env	*node_env;

	i = 0;
	if (envp == NULL)
		return (*top_env = NULL, 1);
	while (envp[i])
	{
		temp = split_env(envp[i]);
		if (!temp)
			return (destructor_env(top_env), *top_env = NULL, 1);
		node_env = create_node_env(temp[0], temp[1]);
		free(temp);
		temp = NULL;
		if (!node_env)
			return (/*ft_putendl_fd(ft_itoa(i), 1), */destructor_env(top_env), pop_env(&node_env), *top_env = NULL, 1);
		push_back_env(top_env, node_env);
		i++;
	}
	return (0);
}
