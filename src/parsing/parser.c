/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 17:16:42 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/29 22:16:54 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

static void	parser_exit_fatal(t_minishell *msh, t_cmd *cmd_h, t_cmd *cur)
{
	if (cur)
		destructor_cmd(&cur);
	if (cmd_h)
		destructor_cmd(&cmd_h);
	free_tokens(msh->top_tok);
	destructor_env(&msh->top_env);
	exit(-1);
}

int	parse_redir(t_tcur *c, t_cmd *cur_cmd, t_cmd *cmd_h, t_minishell *msh)
{
	t_redirect	*tail;

	if (c->current->next->type != WORD || !c->current)
		return (0);
	if (!cur_cmd->redir)
	{
		cur_cmd->redir = create_node_redir(ft_strdup(c->current->next->lexeme),
				c->current->type);
		if (!cur_cmd->redir)
			parser_exit_fatal(msh, cmd_h, cur_cmd);
	}
	else
	{
		tail = cur_cmd->redir;
		while (tail->next)
			tail = tail->next;
		tail->next = create_node_redir(ft_strdup(c->current->next->lexeme),
				c->current->type);
		if (!tail->next)
			parser_exit_fatal(msh, cmd_h, cur_cmd);
	}
	cur_next(c);
	return (1);
}

int	parse_cmd(t_tcur *c, t_cmd *cur_cmd, t_cmd *cmd_h, t_minishell *msh)
{
	t_argv_buf	avb;

	argv_buf_init(&avb);
	while (c->current->type == WORD || is_token_redir(c->current))
	{
		if (c->current->type == WORD)
		{
			if (!argv_buf_push(&avb, ft_strdup(c->current->lexeme)))
				parser_exit_fatal(msh, cmd_h, cur_cmd);
		}
		else if (is_token_redir(c->current))
		{
			if (!parse_redir(c, cur_cmd, cmd_h, msh))
				return (argv_buf_free(&avb), 0);
		}
		cur_next(c);
	}
	cur_cmd->args = argv_buf_end(&avb);
	if (!cur_cmd->args)
		parser_exit_fatal(msh, cmd_h, cur_cmd);
	return (1);
}

t_cmd	*parser(t_minishell *msh)
{
	t_tcur	c;
	t_cmd		*cmd_h;
	t_cmd		*cmd_node;

	c.current = msh->top_tok;
	cmd_h = NULL;
	cmd_node = create_node_cmd(0);
	if (!cmd_node)
		parser_exit_fatal(msh, NULL, NULL);
	if (c.current->type == PIPE)
		return (syntax_error(c.current, msh->top_tok, cmd_h, cmd_node), NULL);
	while (c.current != NULL && c.current->type != END)
	{
		if (c.current->type == WORD)
		{
			if (!parse_cmd(&c, cmd_node, cmd_h, msh))
				return (syntax_error(c.current, msh->top_tok, cmd_h, cmd_node),
					NULL);
		}
		else if (is_token_redir(c.current))
		{
			if (!parse_redir(&c, cmd_node, cmd_h, msh))
				return (syntax_error(c.current, msh->top_tok, cmd_h, cmd_node),
					NULL);
			cur_next(&c);
		}
		else if (c.current->type == PIPE)
		{
			if (c.current->next->type == PIPE || c.current->next->type == END)
				return (syntax_error(c.current->next, msh->top_tok, cmd_h,
						cmd_node), NULL);
			push_back_cmd(&cmd_h, cmd_node);
			cmd_node = create_node_cmd(0);
			if (!cmd_node)
				parser_exit_fatal(msh, cmd_h, NULL);
			if (c.current != NULL)
				cur_next(&c);
		}
	}
	push_back_cmd(&cmd_h, cmd_node);
	free_tokens(msh->top_tok);
	return (cmd_h);
}

/*static int	parser_loop(t_tcur *c, t_minishell *msh, t_cmd *cmd_h,
		t_cmd *cmd_node)
{
	while (c->current != NULL && c->current->type != END)
	{
		if (c->current->type == WORD)
		{
			if (!parse_cmd(c, cmd_node, cmd_h, msh))
				return (0);
		}
		else if (is_token_redir(c->current))
		{
			if (!parse_redir(c, cmd_node, cmd_h, msh))
				return (0);
			cur_next(c);
		}
		else if (c->current->type == PIPE)
		{
			if (c->current->next->type == PIPE || c->current->next->type == END)
				return (0);
			push_back_cmd(&cmd_h, cmd_node);
			cmd_node = create_node_cmd(0);
			if (!cmd_node)
				parser_exit_fatal(msh, cmd_h, NULL);
			if (c->current != NULL)
				cur_next(c);
		}
	}
	return (1);
}

t_cmd	*parser(t_minishell *msh)
{
	t_tcur	c;
	t_cmd	*cmd_h;
	t_cmd	*cmd_node;

	c.current = msh->top_tok;
	cmd_h = NULL;
	cmd_node = create_node_cmd(0);
	if (!cmd_node)
		parser_exit_fatal(msh, NULL, NULL);
	if (c.current->type == PIPE)
		return (syntax_error(c.current, msh->top_tok, cmd_h, cmd_node), NULL);
	if (!parser_loop(&c, msh, cmd_h, cmd_node))
		return (syntax_error(c.current, msh->top_tok, cmd_h, cmd_node), NULL);
	push_back_cmd(&cmd_h, cmd_node);
	free_tokens(msh->top_tok);
	return (cmd_h);
}*/
