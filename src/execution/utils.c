/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 10:04:19 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/22 11:53:51 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief just a function to update the _ (underscore env variable)
 * not really necessary in minishell but in doubt i set it up 
 * because it was kind of easy
 * @param minishell 
 * @return 
 */
int	update_underscore(t_minishell *minishell)
{
	int		i;
	t_env	*temp;
	char	*str;
	char	**argv;

	i = 0;
	argv = minishell -> top_cmd -> args;
	if (minishell -> top_cmd -> args == NULL)
		return (1);
	while (argv[i])
	{
		if (argv[i + 1] != NULL)
			i++;
		else
			break ;
	}
	temp = research_node_env(minishell -> top_env, "_");
	if (temp != NULL)
	{
		str = ft_strdup(argv[i]);
		if (!str)
			return (1);
		(free(temp -> value), temp -> value = str);
	}
	return (0);
}

/**
 * @brief function to update g_exit_code
 * to be the last exit code of the function
 *
 * @param last 
 */
void	update(pid_t last)
{
	pid_t	n;
	int		status;

	n = 1;
	while (n > 0)
	{
		if (n == last && g_exit_code == 0)
		{
			if (WIFEXITED(status))
				g_exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_exit_code = 128 + WTERMSIG(status);
		}
		n = waitpid(-1, &status, 0);
	}
}
