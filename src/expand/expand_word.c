/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 20:20:57 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/21 21:42:59 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

/**
 * @brief Initialise expand struct
 */
static void	init_expand(t_expand *exp)
{
	exp->inquotes = 0;
	exp->qstate = 0;
	exp->argvindex = 0;
}

/**
 * @brief Dequotes and manage quote state to expand correctly
 *
 * @param lex Lexing tool to scan input
 * @param sb String buffer to build output
 * @param exp Variables needed for saving state
 */
static void	handle_quotes(t_lexer *lex, t_str_buf *sb, t_expand *exp)
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
		if ((lex->c == '\'' && exp->qstate == 1) || (lex->c == '"'
				&& exp->qstate == 2))
		{
			exp->qstate = 0;
			exp->inquotes = 0;
			advance(lex, 1);
		}
		else
			(str_buf_putc(sb, lex->c), advance(lex, 1));
	}
}

/**
 * @brief Scans the word to dequote/expand and build correct output 
 *
 * @param str Input that need dequote/expand 
 * @param argvindex Current av index to keep track on which string 
 * I need to delete on not found env variable 
 * @return Returns the new string expanded and dequoted
 */
char	*expand_word(char *str, t_minishell *msh, int argvindex)
{
	t_expand	exp;
	t_lexer		lex;
	t_str_buf	sb;

	init_expand(&exp);
	exp.argvindex = argvindex;
	lex_init(&lex, str);
	str_buf_init(&sb);
	while (lex.c)
	{
		if (lex.c == '\'' || lex.c == '"')
		{
			handle_quotes(&lex, &sb, &exp);
			continue ;
		}
		if (lex.c == '$')
		{
			handle_dollar(&lex, &sb, &exp, msh);
			continue ;
		}
		(str_buf_putc(&sb, lex.c), advance(&lex, 1));
	}
	ft_free((void **)&str);
	return (str_buf_end(&sb));
}
