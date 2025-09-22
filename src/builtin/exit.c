/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 09:08:43 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/22 08:59:05 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_exit(char *str)
{
	while (*str)
	{
		if (ft_isdigit(*str) != 1)
			return (1);
		str++;
	}
	return (0);
}

int	my_exit(char **argv, t_minishell *minishell)
{
	unsigned char	exit_code;

	exit_code = 0;
	if (len(argv) > 1)
	{
		if (check_exit(argv[0]) == 1)
		{
			ft_putstr_fd("Numeric argument required\n", 2);
			destructor_env(&minishell -> top_env);
			destructor_cmd(&minishell -> top_cmd);
			exit(2);
		}
		else
			return (ft_putstr_fd("Too much arguments\n", 2), 1);
	}
	if (argv[0])
		exit_code = (unsigned char) ft_atoi(argv[0]);
	destructor_env(&minishell -> top_env);
	destructor_cmd(&minishell -> top_cmd);
	exit(exit_code);
	return (0);
}
