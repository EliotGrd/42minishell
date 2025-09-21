/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 18:07:49 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/10 13:42:07 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	parsing_destructor(t_token *tok_head, t_cmd *cmd_head)
{
	if (tok_head)
		free_tokens(tok_head);
	if (cmd_head)
		destructor_cmd(&cmd_head);
}

void	syntax_error(t_token *tok, t_token *tok_head, t_cmd *cmd_head)
{
	ft_putstr_fd("bash: syntax error near unexpected token `", 2);
	//write(2, tok->lexeme, ft_strlen(tok->lexeme));//j'ai un probleme de strlen ?
	write(2, "'\n", 2);
	parsing_destructor(tok_head, cmd_head);
	(void) tok;
}

/*void	lexing_error(int errcode, t_token *head)
{
	if (errcode == 1)
	{
	}
	else (errcode == 2)
	{
	}
}*/

void	exp_redir_error(t_minishell *msh, t_redirect redir )
{
	write(2, "ambiguous redirect\n", 19);
	(void)redir;
	//error bash: $MACHIN: ambiguous redirect mais need free etc
	msh->index_rm_exp = -1;
	//skip exec
}
