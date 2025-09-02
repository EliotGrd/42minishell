/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multipipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:39:04 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/02 10:12:24 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief as we are going to fork and create process, 
 * the new process will have
 * inherited of all the opened fd. Which is a problem because 
 * we don't need all the 
 * file descriptor open => so te be sure we are going to close 
 * all the heredoc not useful in 
 * our situation
 *
 * @param top_stack 
 * @param current 
 */
void	close_fd_heredocs(t_cmd *top_stack, t_cmd *current)
{
	t_cmd		*tmp;
	t_redirect	*temp_redir;

	tmp = top_stack;
	while (tmp)
	{
		if (tmp != current)
		{
			temp_redir = tmp -> redir;
			while (temp_redir)
			{
				if (temp_redir -> type == HERE_DOC)
					close(temp_redir -> fd);
				temp_redir = temp_redir -> next;
			}
		}
		tmp = tmp->next;
	}
}

void	execute_child(t_cmd *top_stack, t_cmd *temp, t_env *top_env)
{
	command_redirect(temp);
	close_fd_heredocs(top_stack, temp);
	execution_node(temp-> args, top_env);
	destructor_env(&top_env);//pas sur que ca soit necessaire
	destructor_cmd(&top_stack);//mais le mettre ne casse rien
	exit(EXIT_FAILURE);
}

/**
 * @brief it's where the fork happen and sent to be execute
 *
 * @param temp it's the node of t_cmd we are going to execute
 * @param top_stack 
 * @param top_env 
 * @return 
 */
int	multipipe_intermediary_cmd(t_cmd *temp, t_cmd *top_stack, t_env *top_env)
{
	int	childprocess;

	if (temp -> next != NULL)
		if (pipe(temp -> fd) == -1)
			return (-1);
	childprocess = fork();
	if (childprocess == -1)
		return (-1);
	if (childprocess == 0)
		execute_child(top_stack, temp, top_env);
	else
	{
		if (temp -> previous != NULL)
			close_fd_parent(temp);
		ft_close_fd(&temp -> fd[1]);
	}
	return (0);
}

/**
 * @brief in the case of an error in the redirection,
 * we have a case where the 
 * here_doc opened in a previous situation is still 
 * open (the error happening before we
 * go check the fd open of the heredoc). 
 * So if something happen we have to close that 
 * dangling fd
 *
 * @param temp 
 */
void	close_redir_temp(t_cmd *temp)
{
	t_redirect	*redir;

	redir = temp -> redir;
	while (redir)
	{
		if (redir -> type == HERE_DOC)
			ft_close_fd(&redir -> fd);
		redir = redir -> next;
	}
}

/**
 * @brief management of the multipipe (= name when we have at least one pipe)
 * First step is to manage the heredoc
 * Then we are going to go throught each node of the linked list of command
 * Each time we check the redirection => to manage error and create documents
 * If no error => sent to execute (fork will happen anyway)
 *
 * @param top_stack 
 * @param top_env 
 * @return 
 */
int	multipipe_cmd(t_cmd *top_stack, t_env *top_env)
{
	int		status;
	t_cmd	*temp;

	temp = top_stack;
	if (here_doc_management(top_stack) == -1)
		return (destructor_cmd(&top_stack), -1);
	while (temp)
	{
		if (redirection_verification(&temp) != -1)
		{
			if (multipipe_intermediary_cmd(temp, top_stack, top_env) == -1)
				exit(EXIT_FAILURE);//voir ce qu'il faut faire dans ce cas
		}
		else
			close_redir_temp(temp);
		(ft_close_fd(&temp -> fd_in), ft_close_fd(&temp -> fd_out));
		temp = temp -> next;
	}
	while (waitpid(-1, &status, 0) > 0)//changer 0 => WNOHANG 
		;
	if (WIFEXITED(status))
		g_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_code = 128 + WTERMSIG(status);
	return (1);
}
