/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_cmd2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 08:54:33 by bsuger            #+#    #+#             */
/*   Updated: 2025/10/01 16:36:38 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pop_cmd(t_cmd **top)
{
	t_cmd	*temp;

	if (*top)
	{
		temp = *top;
		ft_close_fd(&temp -> fd[0]);
		ft_close_fd(&temp -> fd[1]);
		ft_close_fd(&temp -> fd_in);
		ft_close_fd(&temp -> fd_out);
		(*top) = (*top)-> next;
		if (temp -> args != NULL)
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
