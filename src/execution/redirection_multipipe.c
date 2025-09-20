/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_multipipe.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:44:13 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/20 16:16:13 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief as fd are all setup to -1 at the beginning 
 * we check if they are open before trying to close 
 * we avoid double close something or close something 
 * which was never open in the first place
 *
 * @param fd 
 */
void	ft_close_fd(int *fd)
{
	if (*fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}

void	close_fd_parent(t_cmd *top_stack)
{
	ft_close_fd(&top_stack -> previous -> fd[0]);
	ft_close_fd(&top_stack -> previous -> fd[1]);
}

void	fail_previous(void)
{
	int	p[2];

	pipe(p);
	ft_close_fd(&p[1]);
	dup2(p[0], STDIN_FILENO);
	ft_close_fd(&p[0]);
}

void	close_previous(t_cmd *top_stack)
{
	if (top_stack->previous)
	{
		ft_close_fd(&top_stack->previous->fd[0]);
		ft_close_fd(&top_stack->previous->fd[1]);
	}
}

/*
 * @brief here the goal is to redirect 
 * everythin as intended
 * if we have a fd_in => it means that 
 * we are not going to read from the 
 * previous command so we take this fd_in 
 * if we don't have a fd_in but we have a previous command =>
 * we have to read in the pevious fd[0] and redirect here
 * if we have a previous node but no fd[0] 
 * => that means something went wrong 
 * in the previous exec/redirect or we had a fd_out 
 * => so we have nothing to take and 
 * tell the command that the stdin is empty
 *
 * if we have a fd_out => we are going to redirect there 
 * but if we haven't a fd_out + 
 * it's not the last command => we redirect inside
 * but if we haven't a fd_out + 
 * it's the last command => we have nothing to do
 * the output is going to be in the terminal
 *
 * in all case we close fd[0] and fd[1] 
 * and the previous node fd
 * @param top_stack 
 */
int	command_redirect(t_cmd *top_stack)
{
	if (top_stack -> fd_in >= 0)
	{
		if (dup2(top_stack -> fd_in, STDIN_FILENO) < 0)
			return (-1);
		ft_close_fd(&top_stack -> fd_in);
	}
	else if (top_stack -> previous && top_stack -> previous -> fd[0] >= 0)
	{
		ft_close_fd(&top_stack -> previous -> fd[1]);
		if (dup2(top_stack -> previous -> fd[0], STDIN_FILENO) < 0)
			return (-1);
		ft_close_fd(&top_stack -> previous -> fd[0]);
	}
	else if (top_stack -> previous)
		fail_previous();
	ft_close_fd(&top_stack -> fd[0]);
	if (top_stack -> fd_out >= 0)
	{
		if (dup2(top_stack -> fd_out, STDOUT_FILENO) < 0)
			return (-1);
		ft_close_fd(&top_stack -> fd_out);
	}
	else if (top_stack -> next)
	{
		if (dup2(top_stack -> fd[1], STDOUT_FILENO) < 0)
			return (-1);
	}
	ft_close_fd(&top_stack -> fd[1]);
	close_previous(top_stack);
	return (0);
}
