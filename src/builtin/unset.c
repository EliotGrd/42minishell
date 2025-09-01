/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:18:51 by bsuger            #+#    #+#             */
/*   Updated: 2025/08/04 11:54:16 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset(t_env **top_env, char *key)
{
	t_env	*temp;

	temp = *top_env;
	while (temp)
	{
		if (ft_strncmp(key, temp -> key, ft_strlen(key) + 1) == 0)
			break ;
		temp = temp -> next;
	}
	if (temp == NULL)
		return ;
	if (temp -> previous != NULL)
		temp -> previous -> next = temp -> next;
	else
		*top_env = temp -> next;
	if (temp -> next != NULL)
		temp -> next -> previous = temp -> previous;
	free(temp -> key);
	free(temp -> value);
	free(temp);
}
