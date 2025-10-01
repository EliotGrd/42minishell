/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dot.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 08:16:19 by bsuger            #+#    #+#             */
/*   Updated: 2025/10/01 09:32:44 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_accessible_dot(char *str)
{
	struct stat	st;
	char		**temp_env;

	temp_env = NULL;
	if (access(str, F_OK) != 0)
		return (message_error(temp_env, str, 0), g_exit_code = 1, 1);
	if (stat(str, &st) == 0 && S_ISDIR(st.st_mode))
		return (message_error(temp_env, str, 1), g_exit_code = 1, 1);
	if (access(str, X_OK) != 0)
		return (message_error(temp_env, str, 2), g_exit_code = 1, 1);
	return (0);
}

static	int	dot_message_error(int n, char *str)
{
	ft_putstr_fd("the terryfying horse is looking at you\n", 2);
	if (n == 1)
	{
		(ft_putstr_fd("cannot execute ; ", 2), ft_putstr_fd(str, 2));
		return (126);
	}
	else if (n == 2)
	{
		(ft_putstr_fd(str, 2), ft_putstr_fd(" file not found\n", 2));
		return (1);
	}
	else if (n == 3)
	{
		ft_putstr_fd(str, 2);
		ft_putstr_fd(" cannot execute binary file\n", 2);
		free(str);
		return (126);
	}
	return (0);
}

int	double_dot(void)
{
	ft_putstr_fd("cat search in the box but didn't fount the command: ..\n", 2);
	return (127);
}

int	single_dot(t_minishell *minishell, char **str)
{
	char	*binary;

	if (str[1])
	{
		if (ft_strchr(str[1], '/'))
		{
			if (check_accessible_dot(str[1]))
				return (g_exit_code);
			return (dot_message_error(1, str[1]));
		}
		binary = search_path(str[1], minishell -> top_env);
		if (!binary)
			return (dot_message_error(2, str[1]));
		return (dot_message_error(3, binary));
	}
	else
	{
		ft_putstr_fd("the terryfying horse is looking at you\n", 2);
		ft_putstr_fd("filename argument is required\n", 2);
		return (2);
	}
}
