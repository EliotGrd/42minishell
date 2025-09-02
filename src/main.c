/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:37:57 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/02 14:27:53 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "parsing.h"
#include "parsing_helpers.h"

int	g_exit_code = 0;//variable global pour les exit code

void	init_minishell(t_minishell *minishell, char **envp)
{
	minishell->top_cmd = NULL;
	minishell->top_env = NULL;
	minishell->top_tok = NULL;
	set_up_env(&minishell->top_env, envp);
}

int	main(int argc, char **argv, char **envp)
{
	banner();
	t_minishell minishell;
	char	*line;

	(void)argc;
	(void)argv;
	init_minishell(&minishell, envp);
	if (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
	{
		while(1)
		{
			signal(SIGINT, sigint_handler);
			signal(SIGQUIT, SIG_IGN);	
			line = readline("CHAT$> ");
			ft_printf("line : %s\n", line);
			if (line == NULL)
			{
				//faudra call la fonction exit ou de destruction global
				ft_printf("Exit\n");
				break;
			}
			if (line && *line)
			{
				add_history(line);
				minishell.top_tok = lexer(line);
				if (minishell.top_tok)
					minishell.top_cmd = parser(minishell.top_tok);
				if (minishell.top_cmd)
					executor(minishell.top_cmd, minishell.top_env);			
			}
			//ici le destructor cree un probleme
			destructor_cmd(&minishell.top_cmd); 
			ft_free((void **)&line);
		}
	}
}
