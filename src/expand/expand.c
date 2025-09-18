/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:36:33 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/18 16:14:33 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

int is_valid_for_key(char c)
{
	if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <= 57) || c == 95)
		return (1);
	return (0);
}

int	is_key_valid(char *str)
{
	int i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] >= 48 && str[0] <= 57)
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!is_valid_for_key(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	handle_quotes(t_lexer *lex, t_str_buf *sb, t_expand *exp)
{
	if (exp->inquotes == 0)
	{
		if (lex->c == '\'')
			exp->qstate = 1;
		else if (lex->c == '"')
			exp->qstate = 2;
		exp->inquotes = 1;
		advance(lex, 1);
	}
	else
	{
		if ((lex->c == '\'' && exp->qstate == 1) || (lex->c == '"' && exp->qstate == 2))
		{
			exp->qstate = 0;
			exp->inquotes = 0;
			advance(lex, 1);
		}
		else
			(str_buf_putc(sb, lex->c), advance(lex, 1));
	}
}

//mega facile a normer
void	handle_dollar(t_lexer *lex, t_str_buf *sb, t_expand *exp, t_minishell *msh)
{
	t_str_buf sb_env;
	char *temp;
	char *expand;

	str_buf_init(&sb_env);
	if (peek(lex) == '?')
	{
		if (exp->qstate != 1)
		{
			str_buf_puts(sb, ft_itoa(g_exit_code));
			advance(lex, 2);
		}
		else
			(str_buf_putc(sb, lex->c), advance(lex, 1));
	}
	else
	{
		if (exp->qstate != 1)
		{
			advance(lex, 1);
			//while (lex->c != ' ' && lex->c != '\'' && lex->c != '"' && lex->c)
			if (lex->c >= 48 && lex->c <= 57)
				(str_buf_putc(&sb_env, lex->c), advance(lex, 1));
			else
			{
				while (lex->c && is_valid_for_key(lex->c))
				{
					str_buf_putc(&sb_env, lex->c);
					advance(lex, 1);
				}
			}
			temp = str_buf_end(&sb_env);
			expand = research_key_env(msh->top_env, temp);//erreur detecte ici car il peut y avoir un NULL du coup segfault
			if (!expand)//correction de l'erreur
				return (ft_free((void **)&temp));
			str_buf_puts(sb, expand);
			//advance(lex, 1);
			ft_free((void **)&temp);
			ft_free((void **)&expand);
		}
		else
			(str_buf_putc(sb, lex->c), advance(lex, 1));
	}
}

char *expand_word(char *str, t_minishell *msh)
{
	t_expand exp;
	t_lexer lex;
	t_str_buf sb;

	exp.inquotes = 0;
	exp.qstate = 0;
	lex_init(&lex, str);
	str_buf_init(&sb);
	while (lex.c)
	{
		if (lex.c == '\'' || lex.c == '"')
		{
			handle_quotes(&lex, &sb, &exp);
			continue;
		}
		if (lex.c == '$')
		{
			handle_dollar(&lex, &sb, &exp, msh);
			continue;
		}
		(str_buf_putc(&sb, lex.c), advance(&lex, 1));
	}
	ft_free((void **)&str);
	return (str_buf_end(&sb));
}

void	expand_manager(t_cmd *head, t_minishell *msh)
{
	t_cmd *cur;
	t_redirect *cur_redir;
	int	i;

	cur = head;
	while (cur)
	{
		i = 0;
		while (cur -> args && cur->args[i])
		{
			cur->args[i] = expand_word(cur->args[i], msh);
			i++;
		}
		cur_redir = cur->redir;
		while (cur_redir)
		{
			cur_redir->file = expand_word(cur_redir->file, msh);
			cur_redir = cur_redir -> next;
		}
		cur = cur->next;
	}
}
