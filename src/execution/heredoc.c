/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 10:39:26 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/20 13:50:19 by bsuger           ###   ########.fr       */
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

int	write_to_heredoc(int *fd, char *line, t_minishell *minishell)
{
	t_env	*temp;
	char	*ptr;
	char	*key;

	if (ft_strchr(line, '$') == NULL)
		write(*fd, line, ft_strlen(line));
	else
	{
		while (*line)
		{
			while (*line && *line != '$')
			{
				write(*fd, line, 1);
				line++;
			}
			if (*line && *(line + 1) && (ft_isalnum(*(line + 1)) || *(line + 1) == '_'))
			{
				ptr = ++line;
				while (*ptr && (ft_isalnum(*ptr) || *ptr == '_'))
					ptr++;
				key = ft_strndup(line, ptr - line);
				if (!key)
					return (1);//a voir comment bien quitter dans ce cas la
				temp = research_node_env(minishell -> top_env, key);
				free(key);
				if (temp != NULL)
					write(*fd, temp -> value, ft_strlen(temp -> value));
				line = ptr;
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
static void	read_gnl_heredoc(t_redirect *temp, t_cmd *top_cmd, t_minishell *minishell, int op)
{
	char	*line;

	while (1)
	{
		write(1, ">", 1);
		line = get_next_line(STDIN_FILENO);
		if (g_exit_code == 130 || g_exit_code == 131)
		{
			ft_close_fd(&op);
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
		write_to_heredoc(&op, line, minishell);
		free(line);
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
static int	update_status(int status, t_redirect *temp, t_cmd *top_cmd)
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
	int			status;
	pid_t		childpid;

	g_exit_code = 0;
	int op = open("./test.txt",O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (op < 0)
		return (-1);
	childpid = fork();
	if (childpid == -1)
		return (-1);
	(signal(SIGQUIT, SIG_IGN), signal(SIGINT, SIG_IGN));
	if (childpid == 0)
	{
		signal(SIGINT, sigint_heredoc);
		signal(SIGQUIT, sigquit_handler);
		remove_echoctl();
		read_gnl_heredoc(temp, minishell -> top_cmd, minishell, op);
		ft_close_fd(&op);
		close_fd_heredocs2(temp, minishell -> top_cmd);
		(destructor_env(&minishell -> top_env), destructor_cmd(&minishell -> top_cmd));
		exit(0);
	}
	waitpid(childpid, &status, 0);
	if (update_status(status, temp, minishell -> top_cmd) == -1)
		return (ft_close_fd(&op), unlink("./test.txt"), -1);
	ft_close_fd(&op);
	op = open("./test.txt",O_RDONLY, 0644);
	if (op < 0)
		return (unlink("./test.txt"), -1);
	unlink("./test.txt");
	return (op);
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
