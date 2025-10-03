/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:37:57 by bsuger            #+#    #+#             */
/*   Updated: 2025/10/01 16:57:26 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "parsing.h"
#include "parsing_helpers.h"

int	g_exit_code = 0;

void	init_minishell(t_minishell *minishell, char **envp)
{
	minishell->top_cmd = NULL;
	minishell->top_env = NULL;
	minishell->top_tok = NULL;
	minishell->index_rm_exp = -1;
	set_up_env(&minishell->top_env, envp);
}

void	quit(t_minishell *minishell)
{
	destructor_cmd(&minishell -> top_cmd);
	destructor_env(&minishell -> top_env);
	ft_printf("Cat finally found his toy, you can leave now..... LEAVE\n");
}

void	line_treatment(char *line, t_minishell *minishell)
{
	add_history(line);
	minishell -> top_tok = lexer(line);
	if (minishell -> top_tok)
		minishell ->top_cmd = parser(minishell);
	if (minishell ->top_cmd)
		expand_manager(minishell -> top_cmd, minishell);
	if (minishell -> top_cmd)
		executor(minishell);
}

int	main(int argc, char **argv, char **envp)
{
	t_minishell	minishell;
	char		*line;

	banner();
	((void)argc, (void)argv);
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	{
		init_minishell(&minishell, envp);
		while (1)
		{
			signal(SIGINT, sigint_handler);
			signal(SIGQUIT, SIG_IGN);
			line = readline("CHAT$> ");
			if (line == NULL)
			{
				quit(&minishell);
				break ;
			}
			if (line && *line)
				line_treatment(line, &minishell);
			destructor_cmd(&minishell.top_cmd);
			ft_free((void **)&line);
		}
	}
}
