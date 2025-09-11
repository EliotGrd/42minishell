/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_verification.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 09:11:55 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/11 08:56:24 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief we close the fd_in and fd_out in case of a fail
 * this function is able to manage the error coming from any open
 * case from infile or outfile or append
 * @param top_stack 
 */
void	close_fd_error(t_cmd **top_stack)
{
	ft_close_fd(&(*top_stack)-> fd_in);
	ft_close_fd(&(*top_stack)-> fd_out);
	if ((*top_stack)-> previous)
	{
		ft_close_fd(&(*top_stack)-> previous -> fd[0]);
		ft_close_fd(&(*top_stack)-> previous -> fd[1]);
	}
}

/**
 * @brief we have to check if the file exist then if 
 * we are allowed to open it
 * in any error case we write a message with the name 
 * of the file who create this situation
 * and leave with the error being catch in an other function
 *
 * @param redir 
 * @param top_stack 
 * @return 
 */
int	infile_redirection(t_redirect *redir, t_cmd **top_stack)
{
	ft_close_fd(&(*top_stack)-> fd_in);
	if (access(redir -> file, F_OK) != 0)
	{
		ft_putstr_fd(redir -> file, 2);
		ft_putstr_fd(" file does not exist\n", 2);
		close_fd_error(top_stack);
		return (-1);
	}
	else if (access(redir -> file, R_OK) != 0) //a verifier si bon flag
	{
		ft_putstr_fd(redir -> file, 2);
		ft_putstr_fd(" permission denied\n", 2);
		close_fd_error(top_stack);
		return (-1);
	}
	(*top_stack)-> fd_in = open(redir -> file, O_RDONLY);
	if ((*top_stack)-> fd_in == -1)
	{
		ft_putstr_fd(redir -> file, 2);
		ft_putstr_fd(" error opening\n", 2);
		close_fd_error(top_stack);
		return (-1);
	}
	return (0);
}

/**
 * @brief outfile and append are open with the right flags
 * in a case of an error we write a message with the name of the file which
 * creates this error
 * we close the previous fd_out and we keep the new one
 *
 * @param redir 
 * @param top_stack 
 * @param type 
 * @return 
 */
int	outfile_append_redirection(t_redirect *redir, t_cmd **top_stack, int type)
{
	ft_close_fd(&(*top_stack)-> fd_out);
	if (type == OUTFILE)
		(*top_stack)-> fd_out = open(redir -> file,
				O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (type == APPEND)
		(*top_stack)-> fd_out = open(redir -> file,
				O_CREAT | O_WRONLY | O_APPEND, 0644);
	if ((*top_stack)-> fd_out == -1)
	{
		ft_putstr_fd(redir -> file, 2);
		ft_putstr_fd(" permission denied\n", 2);
		close_fd_error(top_stack);
		return (-1);
	}
	return (0);
}

/**
 * @brief we go through each node of the redirection 
 * and we have special treatment
 * depending of the following case ; infile, outfile, 
 * apppend and heredoc
 *
 * @param top_stack 
 * @return if an error occur we are going to return -1 
 * and it will be manage in multipipe function
 * as we don't have to execute the command (still we have to
 * be careful with here_doc open in a previous
 * step => done)
 */
int	redirection_verification(t_cmd **top_stack)
{
	t_redirect	*redir;

	redir = (*top_stack)-> redir;
	while (redir)
	{
		if (redir -> type == INFILE)
		{
			if (infile_redirection(redir, top_stack) == -1)
				return (-1);
		}
		else if (redir -> type == OUTFILE || redir -> type == APPEND)
		{
			if (outfile_append_redirection(redir, top_stack,
					redir -> type) == -1)
				return (-1);
		}
		else
		{
			ft_close_fd(&(*top_stack)-> fd_in);
			(*top_stack)-> fd_in = redir -> fd;
		}
		redir = redir -> next;
	}
	return (0);
}
