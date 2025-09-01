/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 14:18:54 by bsuger            #+#    #+#             */
/*   Updated: 2025/08/30 14:19:40 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief remove the echoctl which are the symbol which appears 
 * when you press CTRL+C or CTRL+\
 * it's different here because we don't have to manage this 
 * in readline in particular but only here on here_doc because 
 * we are using GNL
 */
void	remove_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

/**
 * @brief we have to activate back the echoctl to appear again when 
 * we are not inside an heredoc
 * I realize afterward I did not need it because it's only apply
 * to a child process I will kill, I keep it as a safety if needed
 */
void	active_echoctl(void)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
		return ;
	term.c_lflag |= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
