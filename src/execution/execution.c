/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 08:42:26 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/13 10:06:48 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief launch the builtin  
 * some of them are not present because we have to fork() them
 * why ? because we can have redirection, redirection are done in a fork process
 * @param str 
 * @param top_env 
 * @return 
 */
int	launch_builtin(char **str, t_minishell *minishell)
{
	if (ft_strncmp(str[0], "echo", ft_strlen(str[0]) + 1) == 0)
		return (my_echo(++str));
	else if (ft_strncmp(str[0], "pwd", ft_strlen(str[0]) + 1) == 0)
		return (pwd());
	else if (ft_strncmp(str[0], "cd", ft_strlen(str[0]) + 1) == 0)
		return(cd(str, minishell));
	else if (ft_strncmp(str[0], "env", ft_strlen(str[0]) + 1) == 0)
		return(env(minishell -> top_env, str));
	else if (ft_strncmp(str[0], "unset", ft_strlen(str[0]) + 1) == 0)
		return(unset(minishell, str[1]));
	else if (ft_strncmp(str[0], "export", ft_strlen(str[0]) + 1) == 0)
		return(export(str, minishell));
	else if (ft_strncmp(str[0], "exit", ft_strlen(str[0]) + 1) == 0)
		my_exit(++str, minishell);
	else if (ft_strncmp(str[0], ":", ft_strlen(str[0]) + 1) == 0)
		return(0);
	return (0);
}

/**
 * @brief detect if it's a builtin with strncmp()
 * but the real question is "is it a builtin I have 
 * to launch in a fork process or not ?"
 * @param str = the command to check
 * @return 1 if it's a builtin 0 if not
 */
int	is_it_builtin(char *str)
{
	if (ft_strncmp(str, "echo", ft_strlen(str) + 1) == 0
		|| ft_strncmp(str, "pwd", ft_strlen(str) + 1) == 0
		|| ft_strncmp(str, "cd", ft_strlen(str) + 1) == 0
		|| ft_strncmp(str, "env", ft_strlen(str) + 1) == 0
		|| ft_strncmp(str, "unset", ft_strlen(str) + 1) == 0
		|| ft_strncmp(str, "exit", ft_strlen(str) + 1) == 0
		|| ft_strncmp(str, "export", ft_strlen(str) + 1) == 0
		|| ft_strncmp(str, ":", ft_strlen(str) + 1) == 0)
		return (1);
	return (0);
}

/**
 * @brief ici ce sont les builtin qu'on doit pas 
 * fork dans le cas d'un one_command
 *
 * @param str 
 * @return 
 */
int	is_it_builtin_nonfork(char *str)
{
	if (ft_strncmp(str, "cd", ft_strlen(str) + 1) == 0
		|| ft_strncmp(str, "unset", ft_strlen(str) + 1) == 0
		|| ft_strncmp(str, "exit", ft_strlen(str) + 1) == 0
		|| ft_strncmp(str, "export", ft_strlen(str) + 1) == 0)
		return (1);
	return (0);
}

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
	//ici je pense mettre le sigquit
	if (childprocess == -1)
		return (-1);
	if (childprocess == 0)
	{
		signal(SIGQUIT, sigquit_handler);//sigquit
		command_redirect(minishell -> top_cmd);
		execution_node(minishell -> top_cmd -> args, minishell);
		destructor_env(&minishell -> top_env);
		destructor_cmd(&minishell -> top_cmd);
		exit(g_exit_code);
	}
	ft_close_fd(&minishell -> top_cmd -> fd_in);
	ft_close_fd(&minishell -> top_cmd -> fd_out);
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
	if (redirection_verification(&minishell -> top_cmd) != -1)
	{
		if (minishell -> top_cmd -> args == NULL)
			;
		else if (is_it_builtin_nonfork(minishell ->top_cmd -> args[0]))
			g_exit_code = launch_builtin(minishell -> top_cmd -> args, minishell);
		else
			one_command_execve(minishell);
	}
	else
		close_redir_temp(minishell -> top_cmd);
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
	if (cmd_lst_size(minishell -> top_cmd) == 1)
		one_command(minishell);
	else
		multipipe_cmd(minishell);
}
