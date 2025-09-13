/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/13 09:08:43 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/13 10:00:28 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	my_exit(char **argv, t_minishell *minishell)
{
	unsigned char	exit_code;

	exit_code = 0;
	if (len(argv) > 1)
		return (ft_putstr_fd("Too much arguments\n", 2), 1);
	if (argv[0])
		exit_code = (unsigned char) ft_atoi(argv[0]);
	destructor_env(&minishell -> top_env);
	destructor_cmd(&minishell -> top_cmd);
	exit(exit_code);
	return (0);
}
