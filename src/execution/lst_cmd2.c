/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_cmd2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 08:54:33 by bsuger            #+#    #+#             */
/*   Updated: 2025/08/30 12:15:36 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pop_cmd(t_cmd **top)
{
	t_cmd	*temp;

	if (*top)
	{
		temp = *top;
		(*top) = (*top)-> next;
		ft_free_tab(temp -> args);
		destructor_redir(&temp -> redir);
		ft_free((void **)&temp);
	}
}

void	destructor_cmd(t_cmd **top)
{
	while (*top)
		pop_cmd(top);
	*top = NULL;
}
