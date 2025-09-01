/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/04 15:52:15 by bsuger            #+#    #+#             */
/*   Updated: 2025/08/26 13:35:25 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	redir_lst_size(t_redirect *top)
{
	int	i;

	i = 0;
	if (top)
	{
		i++;
		top = top -> next;
	}
	return (i);
}

t_redirect	*get_last_redir(t_redirect **top)
{
	t_redirect	*temp;

	if (*top)
	{
		temp = *top;
		while (temp -> next != NULL)
			temp = temp -> next;
		return (temp);
	}
	else
		return (NULL);
}

void	push_back_redir(t_redirect **top, t_redirect *redirect)
{
	t_redirect	*temp;

	if (*top)
	{
		temp = get_last_redir(top);
		temp -> next = redirect;
	}
	else
		*top = redirect;
}

void	push_redir(t_redirect **top, t_redirect *redirect)
{
	if (*top)
	{
		redirect -> next = *top;
		*top = redirect;
	}
	else
		*top = redirect;
}

t_redirect	*create_node_redir(char *str, t_type type)
{
	t_redirect	*new;

	new = malloc(sizeof(t_redirect));
	if (!new)
		return (NULL);
	new -> file = str;
	new -> type = type;
	new -> fd = -1;
	new -> next = NULL;
	return (new);
}
