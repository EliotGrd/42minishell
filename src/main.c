/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:37:57 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/22 11:07:41 by bsuger           ###   ########.fr       */
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
	minishell->index_rm_exp = -1;
	set_up_env(&minishell->top_env, envp);
}

void	debug_tok(t_minishell *msh)
{
	t_token *toktemp = msh->top_tok;
	for (;toktemp;toktemp = toktemp->next)
	{
		printf("[TOKEN] : %s %d\n", toktemp->lexeme, toktemp->type);
	}
	printf("\n");
}

void	debug_cmd(t_minishell *msh)
{
	int i = 0;
	t_cmd *cur = msh->top_cmd;
	t_redirect *redir;
	while (cur)
	{
		while (cur->args[i])
		{
			printf("[ARGS][%d] : %s\n", i, cur->args[i]);
			i++;
		}
		if (cur->redir)
		{
			while (cur->redir)
			{
				redir = cur->redir;
				printf("[REDIR] : %s %d\n", redir->file, redir->type);
				redir = redir->next;
			}
		}
		i = 0;
		cur = cur->next;
	}
	printf("\n");
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
			//ft_printf("line : %s\n\n", line);
			if (line == NULL)
			{
				//faudra call la fonction exit ou de destruction global
				destructor_cmd(&minishell.top_cmd);
				destructor_env(&minishell.top_env);
				ft_printf("Cat finally found his toy, you can leave now..... LEAVE\n");
				break;
			}
			if (line && *line)
			{
				add_history(line);
				minishell.top_tok = lexer(line);
				
				//debug_tok(&minishell);	
				
				if (minishell.top_tok)
				{
					minishell.top_cmd = parser(&minishell);
				//	exit(0);
				}

				//debug_cmd(&minishell);
				
				if (minishell.top_cmd) //a voir comment faire en fonction des cas d'erreur meme si aucune erreur connue possible durant la partie expand de mon cote
					expand_manager(minishell.top_cmd, &minishell);
				if (minishell.top_cmd)
					executor(&minishell);
			}
			//ici le destructor cree un probleme
			destructor_cmd(&minishell.top_cmd);
			ft_free((void **)&line);
		}
	}
}
