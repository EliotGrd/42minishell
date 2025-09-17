/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:18:51 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/17 15:15:50 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	unset(t_minishell *minishell, char *key)
{
	t_env	*temp;

	temp = minishell -> top_env;
	if (key == NULL)//securite add
		return (0);
	while (temp)
	{
		if (ft_strncmp(key, temp -> key, ft_strlen(key) + 1) == 0)
			break ;
		temp = temp -> next;
	}
	if (temp == NULL)
		return (0);
	if (temp -> previous != NULL)
		temp -> previous -> next = temp -> next;
	else
		minishell -> top_env = temp -> next;
	if (temp -> next != NULL)
		temp -> next -> previous = temp -> previous;
	free(temp -> key);
	free(temp -> value);
	free(temp);
	return (0);
}

int	unset_call(t_minishell *minishell, char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		unset(minishell, str[i]);
		i++;
	}
	return (0);
}

