/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 22:14:23 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/29 22:15:07 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	cur_next(t_tcur *c)
{
	if (c->current->next)
		c->current = c->current->next;
}

int	is_token_redir(t_token *token)
{
	if (token->type == HERE_DOC || token->type == APPEND
		|| token->type == INFILE || token->type == OUTFILE)
		return (1);
	return (0);
}
