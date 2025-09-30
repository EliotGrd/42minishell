/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 10:04:19 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/22 18:29:31 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/**
 * @brief as fd are all setup to -1 at the beginning 
 * we check if they are open before trying to close 
 * we avoid double close something or close something 
 * which was never open in the first place
 *
 * @param fd 
 */
void	ft_close_fd(int *fd)
{
	if (*fd >= 0)
	{
		close(*fd);
		*fd = -1;
	}
}
