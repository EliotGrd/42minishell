/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 20:22:54 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/21 21:34:18 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

/**
 * @brief Handle $? to expand on current error code
 *
 */
static int	error_code_var(t_lexer *lex, t_str_buf *sb, t_expand *exp)
{
	char *exitcode;

	exitcode = ft_itoa(g_exit_code);
	if (exp->qstate != 1)
	{
		if (!str_buf_puts(sb, exitcode))
			return (0);
		ft_free((void **)&exitcode);
		advance(lex, 2);
	}
	else
	{
		if (!str_buf_putc(sb, lex->c))
			return (0);
		advance(lex, 1);
	}
	return (1);
}

static void exit_dollar(t_minishell *msh, t_str_buf *sb, t_str_buf *exp)
{
	if (exp)
		str_buf_free(exp);
	str_buf_free(sb);
	exit_expand_fatal(msh);
}

/**
 * @brief Manages all possibilities with expand : $?, known and 
 * unknown env variable
 *
 */
void	handle_dollar(t_lexer *lex, t_str_buf *sb, t_expand *exp, t_minishell *msh)
{
	t_str_buf	sb_env;
	char		*temp;
	char		*expand;

	if (!is_valid_for_key(peek(lex)) && peek(lex) != '?' && peek(lex) != '\''
		&& peek(lex) != '"')
	{
		if (!str_buf_putc(sb, lex->c))
			exit_dollar(msh, sb, NULL);
		return (advance(lex, 1));
	}
	if (peek(lex) == '?')
	{
		if (!error_code_var(lex, sb, exp))
			exit_dollar(msh, sb, NULL);
	}
	else
	{
		if (exp->qstate != 1)
		{
			str_buf_init(&sb_env);
			advance(lex, 1);
			if (lex->c >= 48 && lex->c <= 57)
			{
				if (!str_buf_putc(&sb_env, lex->c))
					exit_dollar(msh, sb, &sb_env);
				advance(lex, 1);
			}
			else
			{
				while (lex->c && is_valid_for_key(lex->c))
				{
					if (!str_buf_putc(&sb_env, lex->c))
						exit_dollar(msh, sb, &sb_env);
					advance(lex, 1);
				}
			}
			temp = str_buf_end(&sb_env);
			expand = research_key_env(msh->top_env, temp);
			if (!expand)
			{
				msh->index_rm_exp = exp->argvindex;
				return (ft_free((void **)&temp) /*, str_buf_free(sb), advance(lex, 1)*/); // verif si y a pas plus a free genre SB
			}
			if (!str_buf_puts(sb, expand))
				exit_dollar(msh, sb, NULL);
			ft_free((void **)&temp);
			ft_free((void **)&expand);
		}
		else
		{
			if (!str_buf_putc(sb, lex->c))
				exit_dollar(msh, sb, NULL);
			advance(lex, 1);
		}
	}
}
