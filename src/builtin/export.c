/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 09:33:38 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/22 09:49:01 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_validity_key(char *key)
{
	if (ft_isalpha(key[0]) != 1 && key[0] != '_')
		return (1);
	key++;
	while (*key)
	{
		if (ft_isalnum(*key) == 0 && *key != '_')
			return (1);
		key++;
	}
	return (0);
}

int	add_env(t_minishell *minishell, char *extract_key, char *extract_value,
	t_env *temporary)
{
	extract_key = ft_strdup(extract_key);
	if (extract_value == NULL)
		extract_value = ft_strdup("");
	else
		extract_value = ft_strdup(extract_value);
	if (extract_key == NULL || extract_value == NULL)
	{
		ft_free((void **)&extract_key);
		ft_free((void **)&extract_value);
		return (ft_putendl_fd(MALLOC_ERR, 2), 1);
	}
	temporary = create_node_env(extract_key, extract_value);
	if (temporary == NULL)
	{
		ft_free((void **)&extract_key);
		ft_free((void **)&extract_value);
		return (1);
	}
	push_back_env(&minishell -> top_env, temporary);
	return (0);
}

int	export_env_update(t_minishell *minishell, char *extract_key,
		char *extract_value)
{
	t_env	*temporary;

	temporary = research_node_env(minishell -> top_env, extract_key);
	if (temporary == NULL)
	{
		if (add_env(minishell, extract_key, extract_value, temporary) == 1)
			return (1);
	}
	else
	{
		extract_value = ft_strdup(extract_value);
		if (!extract_value)
			return (ft_putendl_fd(MALLOC_ERR, 2), 1);
		(free(temporary -> value), temporary -> value = extract_value);
	}
	return (0);
}

int	export(t_minishell *minishell, char *str)
{
	char	*extract_key;
	char	*extract_value;

	if (str[0] == '=')
		return (ft_putstr_fd("invalid identifier\n", 2), 1);
	if (ft_strlen(str) == 1)
	{
		if (ft_isalpha(*str) == 0 && *str != '_')
			return (ft_putstr_fd("invalid identifier\n", 2), 1);
	}
	else if (ft_strchr(str, '='))
	{
		extract_key = ft_strtok(str, "=");
		extract_value = &str[ft_strlen(extract_key) + 1];
		if (check_validity_key(extract_key))
			return (ft_putstr_fd("invalid identifier\n", 2), 1);
		if (export_env_update(minishell, extract_key, extract_value))
			return (ft_putstr_fd("error env\n", 2), 1);
	}
	else
	{
		if (check_validity_key(str))
			return (ft_putstr_fd("invalid identifier\n", 2), 1);
	}
	return (0);
}

int	export_call(t_minishell *minishell, char **str)
{
	int	exitcode;

	exitcode = 0;
	while (*str != NULL)
	{
		if (export(minishell, *str))
			exitcode = 1;
		str++;
	}
	return (exitcode);
}
