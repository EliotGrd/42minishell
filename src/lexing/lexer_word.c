/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_word.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:29:22 by egiraud           #+#    #+#             */
/*   Updated: 2025/08/23 17:18:19 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

/**
 * @brief Lexes a segment delimited by single quotes 
 *
 * @return Return 1 when success, 0 if an error occured
 */
static int	single_quoted_segment(t_lexer *lex, t_str_buf *sb)
{
	str_buf_putc(sb, lex->c);
	advance(lex, 1);
	while (lex->c && lex->c != '\'')
	{
		str_buf_putc(sb, lex->c);
		advance(lex, 1);
	}
	if (lex->c != '\'')
	{
		// maybe something more to flag/error
		return (0);
	}
	str_buf_putc(sb, lex->c);
	advance(lex, 1);
	return (1);
}

static int	is_dquotes_escapable(char c)
{
	if (c == '$' || c == '`' || c == '"' || c == '\\' || c == '\n')
		return (1);
	return (0);
}

/*static void	escape_in_dquote(t_lexer *lex, t_str_buf *sb, char c)
{
	if (c == '\n')

}*/

/**
 * @brief Lexes a segment delimited by double quotes
 *
 * @return Return 1 when success, 0 if an error occured
 */
static int	double_quoted_segment(t_lexer *lex, t_str_buf *sb)
{
	char	c;

	(str_buf_putc(sb, lex->c), advance(lex, 1));
	while (lex->c && lex->c != '"')
	{
		if (lex->c == '\\')
		{
			c = peek(lex);
			if (!c)
				return (0); // and error unclosed quotes mes couilles
			if (is_dquotes_escapable(c))
			{
				if (c == '\n')
				{
					advance(lex, 2);
					continue;
				}
				(str_buf_putc(sb, lex->c), advance(lex, 2));
				continue ;
			}
			(str_buf_putc(sb, lex->c), advance(lex, 2));
			continue ;
		}
		(str_buf_putc(sb, lex->c), advance(lex, 1));
	}
	if (lex->c != '"')
		return (0); // plus error unclosed quotes
	(str_buf_putc(sb, lex->c), advance(lex, 1));
	return (1);
}

/**
 * @brief Handle backslash to escape next char correctly
 *
 * @return Return 1 when success, 0 if an error occured
 */
static int	escape_segment(t_lexer *lex, t_str_buf *sb)
{
	char	c;

	c = peek(lex);
	if (!c)
	{
		// error "backslash a la fin de l'input" ou un truc du genre
		return (0);
	}
	if (c == '\n')
		return (advance(lex, 2), 1);
	str_buf_putc(sb, c);
		// incertitude mais trop fatigue pour le faire au mental
	advance(lex, 2);
	return (1);
}

/**
 * @brief Lexes the string to create a WORD token
 *
 * @return Returns the token created 
 */
t_token	*lex_word(t_lexer *lex)
{
	t_str_buf		sb;
	unsigned int	q_state;
	t_token *token;
	char *result;

	str_buf_init(&sb);
	while (lex->c && !ft_iswspace(lex->c) && !is_char_operator(lex->c))
	{
		if (lex->c == '\'')
		{
			q_state = 1;
			if (!single_quoted_segment(lex, &sb))
				return (str_buf_free(&sb), NULL);
		}
		else if (lex->c == '\"')
		{
			q_state = 2;
			if (!double_quoted_segment(lex, &sb))
				return (str_buf_free(&sb), NULL);
		}
		else if (lex->c == '\\')
		{
			if (!escape_segment(lex, &sb))
				return (str_buf_free(&sb), NULL);
		}
		else
			(str_buf_putc(&sb, lex->c), advance(lex, 1));
	}
	result = str_buf_end(&sb);
	token = make_token(WORD, result, q_state);
	ft_free((void **)&result);
	return (token);
}
