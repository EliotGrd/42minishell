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

	if (!is_valid_for_key(peek(lex)) && peek(lex) != '?' && peek(lex) != '\'' && peek(lex) != '"')
		return (str_buf_putc(sb, lex->c), advance(lex, 1));
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
			expand = research_key_env(msh->top_env, temp);
			if (!expand)
			{
				msh->index_rm_exp = exp->argvindex;
				return (ft_free((void **)&temp)/*, str_buf_free(sb), advance(lex, 1)*/); //verif si y a pas plus a free genre SB
			}
			str_buf_puts(sb, expand);
			ft_free((void **)&temp);
			ft_free((void **)&expand);
		}
		else
			(str_buf_putc(sb, lex->c), advance(lex, 1));
	}
}

static void	init_expand(t_expand *exp)
{
	exp->inquotes = 0;
	exp->qstate = 0;
	exp->argvindex = 0;
}

char *expand_word(char *str, t_minishell *msh, int argvindex)
{
	t_expand exp;
	t_lexer lex;
	t_str_buf sb;

	init_expand(&exp);
	exp.argvindex = argvindex;
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

static void	argv_remove_index(char **argv, t_minishell *msh)
{
	int	i;

	if (!argv)
		return;
	if (argv[msh->index_rm_exp][0] != '$' && argv[msh->index_rm_exp][0])
	{
		msh->index_rm_exp = -1;
		return ;
	}
	ft_free((void **)&argv[msh->index_rm_exp]);
	i = msh->index_rm_exp + 1;
	argv[msh->index_rm_exp] = argv[i];
	while (argv[i])
	{
		argv[i] = argv[i + 1];
		i++;
	}
	msh->index_rm_exp = -1;
}

void	exp_redir_error(t_minishell *msh, t_redirect redir )
{
	write(2, "ambiguous redirect\n", 19);
	(void)redir;
	//error bash: $MACHIN: ambiguous redirect mais need free etc
	msh->index_rm_exp = -1;
	//skip exec
}

/**
 * @brief 
 *
 * @param head 
 * @param msh 
 */
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
			cur->args[i] = expand_word(cur->args[i], msh, i);
			if (msh->index_rm_exp > -1)
				argv_remove_index(cur->args, msh);
			i++;
		}
		cur_redir = cur->redir;
		while (cur_redir)
		{
			cur_redir->file = expand_word(cur_redir->file, msh, 0);
			if (msh->index_rm_exp > -1)
				exp_redir_error(msh, *cur_redir);
			cur_redir = cur_redir -> next;
		}
		cur = cur->next;
	}
}
