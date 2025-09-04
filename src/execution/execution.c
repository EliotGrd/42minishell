/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 08:42:26 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/04 11:02:09 by bsuger           ###   ########.fr       */
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
int	launch_builtin(char **str, t_env *top_env)
{
	if (ft_strncmp(str[0], "echo", ft_strlen(str[0]) + 1) == 0)
		my_echo(++str);
	else if (ft_strncmp(str[0], "pwd", ft_strlen(str[0]) + 1) == 0)
		pwd();
	else if (ft_strncmp(str[0], "cd", ft_strlen(str[0]) + 1) == 0)
		cd(str, top_env);
	else if (ft_strncmp(str[0], "env", ft_strlen(str[0]) + 1) == 0)
		env(top_env);
	else if (ft_strncmp(str[0], "unset", ft_strlen(str[0]) + 1) == 0)
		ft_printf("unset\n");
	else if (ft_strncmp(str[0], "export", ft_strlen(str[0]) + 1) == 0)
		export(str, top_env);
	else if (ft_strncmp(str[0], "exit", ft_strlen(str[0]) + 1) == 0)
		ft_printf("exit\n");
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
		|| ft_strncmp(str, "export", ft_strlen(str) + 1) == 0)
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
int	one_command_execve(t_cmd *top_cmd, t_env *top_env)
{
	int		status;
	pid_t	childprocess;

	childprocess = fork();
	if (childprocess == -1)
		return (-1);
	if (childprocess == 0)
	{
		command_redirect(top_cmd);
		execution_node(top_cmd -> args, top_env);
		destructor_env(&top_env);
		destructor_cmd(&top_cmd);
		exit(g_exit_code);
	}
	ft_close_fd(&top_cmd -> fd_in);
	ft_close_fd(&top_cmd -> fd_out);
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
int	one_command(t_cmd *top_cmd, t_env *top_env)
{
	if (here_doc_management(top_cmd) == -1)
		return (-1);
	if (redirection_verification(&top_cmd) != -1)
	{
		if (is_it_builtin_nonfork(top_cmd -> args[0]))
			launch_builtin(top_cmd -> args, top_env);
		else
			one_command_execve(top_cmd, top_env);
	}
	else
		close_redir_temp(top_cmd);//ferme heredoc si un probleme avant
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
void	executor(t_cmd *top_cmd, t_env *top_env)
{
	g_exit_code = 0;
	if (cmd_lst_size(top_cmd) == 1)
		one_command(top_cmd, top_env);
	else
		multipipe_cmd(top_cmd, top_env);
}
