/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 12:16:26 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/08 16:44:50 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sigint_handler(int sig)
{
	(void) sig;
	ft_printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	sigint_handler2(int sig)
{
	(void) sig;
	ft_printf("\n");
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
}
