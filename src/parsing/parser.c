/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/23 17:16:42 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/11 09:08:13 by bsuger           ###   ########.fr       */
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

static void parser_exit_fatal(t_minishell *msh, t_cmd *cmd_head, t_cmd *cur)
{
	if (cur)
		destructor_cmd(&cur);
	if (cmd_head)
		destructor_cmd(&cmd_head);
	free_tokens(msh->top_tok);
	destructor_env(&msh->top_env);
	exit(-1);
}

int	parse_redir(t_tokcursor *c, t_cmd *cur_cmd, t_cmd *cmd_head, t_minishell *msh)
{
	t_redirect	*tail;

	// t_redirect	*node;
	// node = create_node_redir(0, c->current->type);
	if (c->current->next->type != WORD || !c->current)
		return (0);
	if (!cur_cmd->redir)
	{
		cur_cmd->redir = create_node_redir(ft_strdup(c->current->next->lexeme),
				c->current->type);
		if (!cur_cmd->redir)
			parser_exit_fatal(msh, cmd_head, cur_cmd);
	}
	else
	{
		tail = cur_cmd->redir;
		while (tail->next)
			tail = tail->next;
		tail->next = create_node_redir(ft_strdup(c->current->next->lexeme),
				c->current->type);
		if (!tail->next)
			parser_exit_fatal(msh, cmd_head, cur_cmd);
	}
	// cur_next(c);//temp pour comprendre l'erreur
	cur_next(c);
	return (1);
}

int	parse_cmd(t_tokcursor *c, t_cmd *cur_cmd, t_cmd *cmd_head, t_minishell *msh)
{
	t_argv_buf	avb;
	int			empty;

	empty = 1;
	argv_buf_init(&avb);
	while (c->current->type == WORD || is_token_redir(c->current))
	{
		if (c->current->type == WORD)
		{
			if (!argv_buf_push(&avb, ft_strdup(c->current->lexeme)))
				parser_exit_fatal(msh, cmd_head, cur_cmd);
			empty = 0;
		}
		else if (is_token_redir(c->current))
		{
			if (!parse_redir(c, cur_cmd, cmd_head, msh))
				return (argv_buf_free(&avb), 0);
			empty = 0;
		}
		cur_next(c);
	}
	cur_cmd->args = argv_buf_end(&avb);
	if (!cur_cmd->args)
		parser_exit_fatal(msh, cmd_head, cur_cmd);
	if (empty == 1)
		return (argv_buf_free(&avb), 0);
	return (1);
}

t_cmd	*parser(t_minishell *msh)
{
	t_tokcursor	c;
	t_cmd		*cmd_head;
	t_cmd		*cmd_node;

	c.current = msh->top_tok;
	cmd_head = NULL;
	cmd_node = create_node_cmd(0); // ici check de secu a faire
	if (!cmd_node)
		parser_exit_fatal(msh, NULL, NULL);
	if (c.current->type == PIPE)
		return (syntax_error(c.current, msh->top_tok, cmd_head, cmd_node), NULL);
	while (c.current != NULL && c.current->type != END)
	{
		if (c.current->type == WORD)
		{
			if (!parse_cmd(&c, cmd_node, cmd_head, msh))
				return (syntax_error(c.current, msh->top_tok, cmd_head, cmd_node), NULL);
		}
		else if (is_token_redir(c.current))
		{
			if (!parse_redir(&c, cmd_node, cmd_head, msh))
				return (syntax_error(c.current, msh->top_tok, cmd_head, cmd_node), NULL);
			cur_next(&c);
		}
		else if (c.current->type == PIPE)
		{
			if (c.current->next->type == PIPE || c.current->next->type == END)
				return (syntax_error(c.current->next, msh->top_tok, cmd_head, cmd_node), /*ft_free_tab(cmd_node->args),*/ NULL);
			push_back_cmd(&cmd_head, cmd_node);
			cmd_node = create_node_cmd(0); // securite a faire
			if (!cmd_node)
				parser_exit_fatal(msh, cmd_head, NULL);
			if (c.current != NULL)
				cur_next(&c);
		}
	}
	push_back_cmd(&cmd_head, cmd_node); // pas sur de celui la
	free_tokens(msh->top_tok);
	return (cmd_head);
}



				/*c.current = NULL;//ajout de ma part
				//head = NULL;//ajout de ma part
				ft_free((void **)&cmd_node);//ajout de ma part
				//cmd_node = NULL;//ajout de ma part
				return (NULL);*/
