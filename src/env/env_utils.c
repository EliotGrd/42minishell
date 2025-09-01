/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:25:50 by bsuger            #+#    #+#             */
/*   Updated: 2025/07/28 14:26:03 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*get_last_env(t_env **top_env)
{
	t_env	*temp;

	if (*top_env)
	{
		temp = *top_env;
		while (temp -> next != NULL)
			temp = temp -> next;
		return (temp);
	}
	else
		return (NULL);
}

int	env_lst_size(t_env *top_env)
{
	int	i;

	i = 0;
	while (top_env)
	{
		i++;
		top_env = top_env -> next;
	}
	return (i);
}
