/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 11:08:46 by bsuger            #+#    #+#             */
/*   Updated: 2025/08/30 11:09:38 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	cd(char **argv, t_env *top_env)
{
	char	*home;

	if (argv[1] == NULL)
	{
		home = research_key_env(top_env, "HOME");
		if (home == NULL)
		{
			ft_putstr_fd("HOME is not set\n", 2);
			return (1);
		}
		if (chdir(home) == -1)
			perror("CHAT$> ");
		free(home);
		return (1);
	}
	else
	{
		if (chdir(argv[1]) == -1)
			perror("CHAT$> ");
	}
	return (0);
}
