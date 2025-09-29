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
static int	handle_quotes(t_lexer *lex, t_str_buf *sb, t_expand *exp)
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
			//advance(lex, 1);
		}
		else
		{
			if (!str_buf_putc(sb, lex->c))
				return (0);
			//advance(lex, 1);
		}
		advance(lex, 1); //si erreur expand sur les quotes reverse changes
	}
	return (1);
}

void exit_expand_fatal(t_minishell *msh)
{
	destructor_cmd(&msh->top_cmd);
	destructor_env(&msh->top_env);
	exit(-1);
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
	char *result;

	init_expand(&exp);
	exp.argvindex = argvindex;
	lex_init(&lex, str);
	str_buf_init(&sb);
	while (lex.c)
	{
		if (lex.c == '\'' || lex.c == '"')
		{
			if (!handle_quotes(&lex, &sb, &exp))
				(str_buf_free(&sb), exit_expand_fatal(msh));
			continue ;
		}
		if (lex.c == '$')
		{
			handle_dollar(&lex, &sb, &exp, msh);
			continue ;
		}
		if (!str_buf_putc(&sb, lex.c))
			(str_buf_free(&sb), exit_expand_fatal(msh));
		advance(&lex, 1);
	}
	ft_free((void **)&str);
	result = str_buf_end(&sb);
	if (!result)
		exit_expand_fatal(msh);
	return (result);
}
