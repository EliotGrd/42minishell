/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:16:26 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/17 08:20:13 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	(void) sig;
	ft_printf("\n");//ca en trop ? 
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	g_exit_code = 130;
}

/**
 * @brief allow us to don't create issue when a child process with
 * readline 
 *
 * @param sig 
 */
void	sigint_handler2(int sig)
{
	(void) sig;
	ft_printf("\n");//ca en trop ? 
	rl_replace_line("", 0);
	rl_on_new_line();
	//rl_redisplay();//pour pas afficher de nouveau le prompt
}
/**
 * @brief allow us to warn the process child heredoc it's time
 * to stop and close the file descriptor 
 *
 * @param sig 
 */
void	sigint_heredoc(int sig)
{
	(void) sig;
	g_exit_code = 130;
	close(STDIN_FILENO);
	write(1, "^C\n", 3);
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	sigquit_handler(int sig)
{
	(void) sig;
	g_exit_code = 131;
	close(STDIN_FILENO);
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
}
