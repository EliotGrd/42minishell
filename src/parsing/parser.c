/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 17:16:42 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/02 17:26:44 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	cur_next(t_tokcursor *c)
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

int	parse_redir(t_tokcursor *c, t_cmd *cur_cmd)
{
	t_redirect	*node;
	t_redirect	*tail;

	node = create_node_redir(0, c->current->type);
	if (c->current->next->type != WORD || !c->current)
		return (0);
	if (!cur_cmd->redir)
		cur_cmd->redir = create_node_redir(c->current->next->lexeme,
				c->current->type);
	else
	{
		tail = cur_cmd->redir;
		while (tail->next)
			tail = tail->next;
		tail->next = create_node_redir(c->current->next->lexeme,
				c->current->type);
	}
	cur_next(c);
	cur_next(c);
	return (1);
}

int	parse_cmd(t_tokcursor *c, t_cmd *cur_cmd)
{
	t_argv_buf	avb;
	int			empty;

	empty = 1;
	argv_buf_init(&avb);
	while (c->current->type == WORD || is_token_redir(c->current))
	{
		if (c->current->type == WORD)
		{
			argv_buf_push(&avb, ft_strdup(c->current->lexeme));
			empty = 0;
		}
		else if (is_token_redir(c->current))
		{
			if (!parse_redir(c, cur_cmd))
				return (0);
			empty = 0;
		}
		cur_next(c);
	}
	cur_cmd->args = argv_buf_end(&avb);
	if (empty == 1)
	{
		argv_buf_free(&avb);
		return (0);
	}
	return (1);
}

t_cmd	*parser(t_token *head)
{
	t_tokcursor	c;
	t_cmd		*cmd_head;
	t_cmd		*cmd_node;

	c.current = head;
	cmd_node = create_node_cmd(0);//ici check de secu a faire
	cmd_head = NULL; //cmd_node;
	while (c.current->type != END)
	{
		if (c.current->type == WORD)
		{
			if (!parse_cmd(&c, cmd_node))
				syntax_error(c.current, head, cmd_head);
		}
		else if (is_token_redir(c.current))
		{
			if (!parse_redir(&c, cmd_node))
				syntax_error(c.current, head, cmd_head);
		}
		else if (c.current->type == PIPE)
		{
			if (c.current->next->type == PIPE || c.current->next->type == END)
				syntax_error(c.current->next, head, cmd_head);
			push_back_cmd(&cmd_head, cmd_node);
			cmd_node = create_node_cmd(0);//securite a faire
			cur_next(&c);//ajout de ma part
		}
	}
	push_back_cmd(&cmd_head, cmd_node);//pas sur de celui la
	free_tokens(head);
	return (cmd_head);
}

// tant que token est pas pipe ou end ou plus de token
//
