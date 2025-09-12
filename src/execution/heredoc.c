/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:39:26 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/12 13:34:13 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief allow us to close the file descriptors when 
 * we have fork() again the
 * process
 *
 * @param current 
 * @param top_stack 
 */
void	close_fd_heredocs2(t_redirect *current, t_cmd *top_stack)
{
	t_cmd		*tmp;
	t_redirect	*temp_redir;

	tmp = top_stack;
	while (tmp)
	{
		temp_redir = tmp -> redir;
		while (temp_redir)
		{
			if (temp_redir != current && temp_redir -> type == HERE_DOC)
				ft_close_fd(&temp_redir -> fd);
			temp_redir = temp_redir -> next;
		}
		tmp = tmp -> next;
	}
}

/**
 * @brief function which allow me to do the loop 
 * to call GNL => there is also the management to
 * leave with the signal
 * the origin of this function is the norm limitation
 *
 * @param fd 
 * @param temp 
 * @param top_cmd 
 */
static void	read_gnl_heredoc(int fd[2], t_redirect *temp, t_cmd *top_cmd, t_minishell *minishell)
{
	char	*line;

	while (1)
	{
		write(1, ">", 1);
		line = get_next_line(STDIN_FILENO);
		if (g_exit_code == 130 || g_exit_code == 131)
		{
			(ft_close_fd(&fd[0]), ft_close_fd(&fd[1]));
			close_fd_heredocs2(temp, top_cmd);
			destructor_env(&minishell -> top_env);
			destructor_cmd(&minishell -> top_cmd);
			exit(g_exit_code);
		}
		if (!line)
		{
			write(1, "\n", 1);
			break ;
		}
		if (!ft_strncmp(line, temp -> file, ft_strlen(temp -> file))
			&& line[ft_strlen(temp -> file)] == '\n')
		{
			(free(line), get_next_line(-1));
			break ;
		}
		(write(fd[1], line, ft_strlen(line)), free(line));
	}
}

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
static int	update_status(int status, int fd[2],
			t_redirect *temp, t_cmd *top_cmd)
{
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 130)
			g_exit_code = 130;
		else if (WEXITSTATUS(status) == 131)
			g_exit_code = 131;
		if (g_exit_code == 130 || g_exit_code == 131)
		{
			ft_close_fd(&fd[0]);
			ft_close_fd(&fd[1]);
			close_fd_heredocs2(temp, top_cmd);
			return (-1);
		}
	}
	return (0);
}

/**
 * @brief it's where we open the heredoc and start 
 * writing inside our pipe
 * each time we check if it's the word allowing to 
 * stop the heredoc or not
 * we are going to keep only the read part of the fd 
 * inside the node of redirection
 *
 * @param temp 
 * @return
 */

int	heredoc_input(t_redirect *temp, t_minishell *minishell)
{
	int			fd[2];
	int			status;
	pid_t		childpid;

	g_exit_code = 0;
	if (pipe(fd) == -1)
		return (-1);
	childpid = fork();
	if (childpid == -1)
		return (-1);
	(signal(SIGQUIT, SIG_IGN), signal(SIGINT, SIG_IGN));
	if (childpid == 0)
	{
		signal(SIGINT, sigint_heredoc);
		signal(SIGQUIT, sigquit_handler);
		ft_close_fd(&fd[0]);
		remove_echoctl();
		read_gnl_heredoc(fd, temp, minishell -> top_cmd, minishell);
		ft_close_fd(&fd[1]);
		close_fd_heredocs2(temp, minishell -> top_cmd);
		destructor_env(&minishell -> top_env);
		destructor_cmd(&minishell -> top_cmd);
		exit(0);
	}
	(ft_close_fd(&fd[1]), waitpid(childpid, &status, 0));
	if (update_status(status, fd, temp, minishell -> top_cmd) == -1)
		return (-1);
	return (fd[0]);
}

/**
 * @brief the function will go through all the commands 
 * and check the redirection
 * if we encounter a heredoc we manage it 
 *
 * @param top_cmd 
 * @return 
 */
int	here_doc_management(t_minishell *minishell)
{
	t_cmd		*temp_cmd;
	t_redirect	*temp_redir;

	temp_cmd = minishell -> top_cmd;
	while (temp_cmd)
	{
		temp_redir = temp_cmd -> redir;
		while (temp_redir)
		{
			if (temp_redir -> type == HERE_DOC)
			{
				temp_redir -> fd = heredoc_input(temp_redir, minishell);
				if (temp_redir -> fd == -1)
					return (-1);
			}
			temp_redir = temp_redir -> next;
		}
		temp_cmd = temp_cmd -> next;
	}
	return (0);
}
