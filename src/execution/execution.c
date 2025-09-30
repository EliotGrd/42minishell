/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 08:42:26 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/22 10:58:20 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief in this function we do the fork to create a child process
 * we do the redirection if we need to
 *
 * @param top_cmd
 * @param top_env
 * @return
 */
int	one_command_execve(t_minishell *minishell)
{
	int		status;
	pid_t	childprocess;

	childprocess = fork();
	signal(SIGINT, sigint_handler2);
	if (childprocess == -1)
		return (-1);
	if (childprocess == 0)
	{
		signal(SIGQUIT, sigquit_handler);
		if (command_redirect(minishell->top_cmd) == 0)
			execution_node(minishell->top_cmd->args, minishell);
		destructor_env(&minishell->top_env);
		destructor_cmd(&minishell->top_cmd);
		exit(g_exit_code);
	}
	ft_close_fd(&minishell->top_cmd->fd_in);
	ft_close_fd(&minishell->top_cmd->fd_out);
	while (waitpid(-1, &status, 0) > 0)
		;
	if (WIFEXITED(status))
		g_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_code = 128 + WTERMSIG(status);
	return (g_exit_code);
}

/**
 * @brief here we check if it's a builtin where we have to fork or not
 * we still need to do the redirection verification
 * (aka creation and error management of
 * files). In the case of a builtin they are not going to be used
 * but....depends of which builtin
 *
 * @param top_cmd
 * @param top_env
 * @return
 */
int	one_command(t_minishell *minishell)
{
	if (here_doc_management(minishell) == -1)
		return (-1);
	if (redirection_verification(&minishell->top_cmd) != -1)
	{
		if (minishell->top_cmd->args == NULL)
			;
		if (!minishell->top_cmd->args[0]
			|| minishell->top_cmd->args[0][0] == '\0')
			ft_putendl_fd("Cat didnt found the command lol", 2);
		else if (is_it_builtin_nonfork(minishell->top_cmd->args[0]))
			g_exit_code = launch_builtin(minishell->top_cmd->args, minishell);
		else
			one_command_execve(minishell);
	}
	else
		(close_redir_temp(minishell->top_cmd), g_exit_code = 1);
	return (0);
}

/**
 * @brief first function of the execution part when we have something to do
 * we have to deal with two cases ; if we have only one command and more
 * why ? because in the case of one command + with
 * some builtin we don't have to fork
 * while with multipipe (from one | to an infinite number)
 * you have to fork anyway
 * @param top_cmd
 * @param top_env
 */
void	executor(t_minishell *minishell)
{
	g_exit_code = 0;
	if (cmd_lst_size(minishell->top_cmd) == 1)
		one_command(minishell);
	else
		multipipe_cmd(minishell);
}
