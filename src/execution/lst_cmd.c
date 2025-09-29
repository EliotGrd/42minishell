/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 08:52:42 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/02 16:21:13 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*create_node_cmd(char **str)
{
	t_cmd	*new;

	(void)str;
	new = malloc(sizeof(t_cmd));
	if (!new)
		return (ft_putendl_fd(MALLOC_ERR, 2), NULL);
	new -> previous = NULL;
	new -> next = NULL;
	new -> redir = NULL;
	new -> fd_in = -1;
	new -> fd_out = -1;
	new -> fd[0] = -1;
	new -> fd[1] = -1;
	new -> args = NULL;
	return (new);
}

void	push_cmd(t_cmd **top, t_cmd *new)
{
	if (*top)
	{
		new -> next = *top;
		(*top)-> previous = new;
		*top = new;
	}
	else
		*top = new;
}

t_cmd	*get_last_cmd(t_cmd **top)
{
	t_cmd	*temp;

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

void	push_back_cmd(t_cmd **top, t_cmd *new)
{
	t_cmd	*temp;

	if (*top)
	{
		temp = get_last_cmd(top);
		temp -> next = new;
		new -> previous = temp;
	}
	else
		*top = new;
}

int	cmd_lst_size(t_cmd *top)
{
	int	i;

	i = 0;
	while (top)
	{
		i++;
		top = top -> next;
	}
	return (i);
}
