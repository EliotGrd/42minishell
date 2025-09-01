/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 11:37:57 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/01 13:01:31 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_exit_code = 0;//variable global pour les exit code

int	main(int argc, char **argv, char **envp)
{
	banner();

	char	*line;

	signal(SIGINT, sigint_handler);//peut etre pas necessaire
	signal(SIGQUIT, SIG_IGN);	
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
				add_history(line);

			//condition pour voir si la line est remplit et enclenche tout le reste
			executor(top_cmd, top_env);
			//ici il faut free top_cmd au minimum 
			free(line);
		}
	}
}
