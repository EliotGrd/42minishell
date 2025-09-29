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

void	parsing_destructor(t_token *tok_head, t_cmd *cmd_head, t_cmd *cmd_cur)
{
	if (tok_head)
		free_tokens(tok_head);
	if (cmd_head)
		destructor_cmd(&cmd_head);
	if (cmd_cur)
		destructor_cmd(&cmd_cur);
}

static char	*find_redir_symbol(t_token *tok)
{
	if (tok->type == INFILE)
		return (ft_strdup("<"));
	else if (tok->type == OUTFILE)
		return (ft_strdup(">"));
	else if (tok->type == HERE_DOC)
		return (ft_strdup("<<"));
	else if (tok->type == APPEND)
		return (ft_strdup(">>"));
	else if (tok->type == PIPE)
		return (ft_strdup("|"));
	else
		return (ft_strdup("newline"));
}

void	syntax_error(t_token *tok, t_token *tok_head, t_cmd *cmd_head,
		t_cmd *cmd_cur)
{
	char	*print;

	print = find_redir_symbol(tok);
	ft_putstr_fd("syntax error near unexpected token `", 2);
	ft_putstr_fd(print, 2);
	ft_putendl_fd("', cat's laughing at you... looser...", 2);
	ft_free((void **)&print);
	parsing_destructor(tok_head, cmd_head, cmd_cur);
	g_exit_code = 2;
}
