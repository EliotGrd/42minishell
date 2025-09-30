/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:43:38 by bsuger            #+#    #+#             */
/*   Updated: 2025/08/30 16:15:39 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*create_node_env(char *key_env, char *value_env)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (ft_putendl_fd(MALLOC_ERR, 2), NULL);
	new -> previous = NULL;
	new -> next = NULL;
	new -> key = key_env;
	new -> value = value_env;
	return (new);
}

void	push_env(t_env **top_env, t_env *new)
{
	if (*top_env)
	{
		new -> next = *top_env;
		(*top_env)-> previous = new;
		*top_env = new;
	}
	else
		*top_env = new;
}

void	push_back_env(t_env **top_env, t_env *new)
{
	t_env	*temp;

	if (*top_env)
	{
		temp = get_last_env(top_env);
		temp -> next = new;
		new -> previous = temp;
	}
	else
		*top_env = new;
}

void	pop_env(t_env **top_env)
{
	t_env	*temp;

	if (*top_env)
	{
		temp = *top_env;
		(*top_env) = (*top_env)-> next;
		free(temp -> key);
		free(temp -> value);
		free(temp);
	}
}

void	destructor_env(t_env **top_env)
{
	while (*top_env)
		pop_env(top_env);
}
