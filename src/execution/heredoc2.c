/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:52:52 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/22 14:04:53 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief origin of creation ; the norm
 * what it is doing ? 
 * juste close the fd the right way if we 
 * encounter the CTRL+C signal and setup the exit_code
 *
 * @param status 
 * @param fd 
 * @param temp 
 * @param top_cmd 
 * @return 
 */
int	update_status(int status, t_redirect *temp, t_cmd *top_cmd)
{
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
			g_exit_code = 130;
		else if (WEXITSTATUS(status) == 131)
			g_exit_code = 131;
		if (g_exit_code == 130 || g_exit_code == 131)
		{
			close_fd_heredocs2(temp, top_cmd);
			return (-1);
		}
	}
	return (0);
}

int	write_heredoc_expand(char **line, int **fd, t_minishell *minishell)
{
	t_env	*temp;
	char	*ptr;
	char	*key;

	ptr = ++(*line);
	while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
		ptr++;
	key = ft_strndup(*line, ptr - *line);
	if (!key)
		return (1);
	temp = research_node_env(minishell -> top_env, key);
	free(key);
	if (temp != NULL)
		write(**fd, temp -> value, ft_strlen(temp -> value));
	*line = ptr;
	return (0);
}

/**
 * @brief function to write on the heredoc file
 * depending if we have a $ sign or not we write 
 * or we expand
 *
 * @param fd 
 * @param line 
 * @param minishell 
 * @return 
 */
int	write_to_heredoc(int *fd, char *line, t_minishell *minishell)
{
	if (ft_strchr(line, '$') == NULL)
		write(*fd, line, ft_strlen(line));
	else
	{
		while (*line)
		{
			while (*line && *line != '$')
				(write(*fd, line, 1), line++);
			if (*line && *(line + 1) && (ft_isalnum(*(line + 1))
					|| *(line + 1) == '_'))
			{
				if (write_heredoc_expand(&line, &fd, minishell) == 1)
					return (1);
			}
			else if (*line)
			{
				write(*fd, line, 1);
				line++;
			}
		}
	}
	return (0);
}
