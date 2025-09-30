/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multipipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:39:04 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/22 11:52:39 by bsuger           ###   ########.fr       */
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

void	execute_child(t_cmd *temp, t_minishell *minishell)
{
	signal(SIGQUIT, sigquit_handler);
	if (command_redirect(temp) == 0)
	{
		close_fd_heredocs(minishell -> top_cmd, temp);
		execution_node(temp-> args, minishell);
	}
	close_fd_heredocs(minishell -> top_cmd, temp);
	destructor_env(&minishell -> top_env);
	destructor_cmd(&minishell -> top_cmd);
	exit(g_exit_code);
}

/**
 * @brief it's where the fork happen and sent to be execute
 *
 * @param temp it's the node of t_cmd we are going to execute
 * @param top_stack 
 * @param top_env 
 * @return 
 */
int	multipipe_intermediary_cmd(t_cmd *temp, t_minishell *minishell, pid_t *last)
{
	if (temp -> next != NULL)
		if (pipe(temp -> fd) == -1)
			return (-1);
	*last = fork();
	signal(SIGINT, sigint_handler2);
	if (*last == -1)
		return (-1);
	if (*last == 0)
		execute_child(temp, minishell);
	else
	{
		if (temp -> previous != NULL)
			close_fd_parent(temp);
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
int	multipipe_cmd(t_minishell *minishell)
{
	t_cmd	*temp;
	pid_t	last;

	temp = minishell -> top_cmd;
	if (here_doc_management(minishell) == -1)
		return (-1);
	while (temp)
	{
		if (redirection_verification(&temp) != -1)
		{
			g_exit_code = 0;
			if (temp -> args == NULL)
				;
			if (!temp -> args[0] || temp -> args[0][0] == '\0')
				ft_putendl_fd("Cat didnt found the command lol", 2);
			else if (multipipe_intermediary_cmd(temp, minishell, &last) == -1)
				exit(EXIT_FAILURE);
		}
		else
			(close_redir_temp(temp), g_exit_code = 1);
		(ft_close_fd(&temp -> fd_in), ft_close_fd(&temp -> fd_out));
		temp = temp -> next;
	}
	update(last);
	return (1);
}
