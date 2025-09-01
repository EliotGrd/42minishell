/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 10:49:45 by bsuger            #+#    #+#             */
/*   Updated: 2025/08/30 12:02:38 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief error message management for the specific case of execution
 * 
 *
 * @param temp_env 
 * @param str 
 * @param n if 0 it does not exist, if 1 it's a directory, 
 * if 2 we don't have the permission, if 3 we have not found the command 
 * (it's a case where we don't have a path)
 */
static void	message_error(char **temp_env, char *str, int n)
{
	ft_free_tab(temp_env);
	if (n == 0)
	{
		ft_putstr_fd("CHATS: ", 2);
		perror(str);
	}
	else if (n == 1)
	{
		ft_putstr_fd("CHATS: ", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd(" Is a directory\n", 2);
	}
	else if (n == 2)
	{
		ft_putstr_fd("CHATS: ", 2);
		perror(str);
	}
	else if (n == 3)
	{
		ft_putstr_fd("CHATS: ", 2);
		ft_putstr_fd("Command not found: ", 2);
		ft_putendl_fd(str, 2);
	}
}

static int	check_accessible(char *str, char **temp_env)
{
	struct stat	st;

	if (access(str, F_OK) != 0)
		return (message_error(temp_env, str, 0), g_exit_code = 127, 1);
	if (stat(str, &st) == 0 && S_ISDIR(st.st_mode))
		return (message_error(temp_env, str, 1), g_exit_code = 126, 1);
	if (access(str, X_OK) != 0)
		return (message_error(temp_env, str, 2), g_exit_code = 126, 1);
	return (0);
}

/**
 * @brief we are going to execute the command here
 * we have a security about absolute path given => because it's 
 * up to us to change
 * information in specific case.
 * if it's a builtin => we call the right function we have done 
 * the redirection verification and also the redirection set up
 *
 * @param str 
 * @param top_env 
 * @return 
 */
int	execution_node(char **str, t_env *top_env)
{
	char		**temp_env;
	char		*binary;

	temp_env = lst_to_tab_env(top_env);
	if (!temp_env)
		return (-1);//a voir comment gere pour bien quitter 
	//mais dasn un fork donc surement exit()
	if (is_it_builtin(str[0]))
		(launch_builtin(str, top_env), exit(EXIT_SUCCESS));
	else if (ft_strchr(str[0], '/'))
	{
		if (check_accessible(str[0], temp_env) != 0)
			exit(g_exit_code);
		binary = str[0];
	}
	else
	{
		binary = search_path(str[0], top_env);
		if (!binary)
			return (message_error(temp_env, str[0], 3), g_exit_code = 127);
	}
	execve(binary, str, temp_env);
	perror("Error");
	return (0);
}
