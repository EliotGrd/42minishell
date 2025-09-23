/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launcher.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 10:09:37 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/23 13:57:57 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		return (my_echo(str));
	else if (ft_strncmp(str[0], "pwd", ft_strlen(str[0]) + 1) == 0)
		return (pwd());
	else if (ft_strncmp(str[0], "cd", ft_strlen(str[0]) + 1) == 0)
		return (cd(str, minishell));
	else if (ft_strncmp(str[0], "env", ft_strlen(str[0]) + 1) == 0)
		return (env(minishell -> top_env, str));
	else if (ft_strncmp(str[0], "unset", ft_strlen(str[0]) + 1) == 0)
		return (unset_call(minishell, ++str));
	else if (ft_strncmp(str[0], "export", ft_strlen(str[0]) + 1) == 0)
		return (export_call(minishell, ++str));
	else if (ft_strncmp(str[0], "exit", ft_strlen(str[0]) + 1) == 0)
		my_exit(++str, minishell);
	else if (ft_strncmp(str[0], ":", ft_strlen(str[0]) + 1) == 0)
		return (0);
	return (0);
}
