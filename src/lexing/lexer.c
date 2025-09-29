/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:32:01 by egiraud           #+#    #+#             */
/*   Updated: 2025/08/23 17:18:31 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

/*
 * PENSER A RENVOYER DES LINKED LISTS VIDES EN ERROR
 * error unclosed quotes ? ou pas laisser comme ca
 */

/**
 * @brief Check if the char is one of the possible operator,
 * currently '<', '>' or '|'
 */
int	is_char_operator(char c)
{
	if (c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

/**
 * @brief Make the operator in the global string a token
 *
 * @return Return the token made
 */
static t_token	*lex_operator(t_lexer *lex)
{
	if (lex->c == '<')
	{
		if (peek(lex) == '<')
		{
			advance(lex, 2);
			return (make_token(HERE_DOC, NULL));
		}
		advance(lex, 1);
		return (make_token(INFILE, NULL));
	}
	if (lex->c == '>')
	{
		if (peek(lex) == '>')
		{
			advance(lex, 2);
			return (make_token(APPEND, NULL));
		}
		advance(lex, 1);
		return (make_token(OUTFILE, NULL));
	}
	advance(lex, 1);
	return (make_token(PIPE, NULL));
}

/**
 * @brief Global lexing function, that traverses readline's string and
 * separate in tokens
 *
 * @param line String comming from readline
 * @return Return the head of t_token struct so it can be parsed
 */
t_token	*lexer(char *line)
{
	t_lexer	lex;
	t_token	*head;
	t_token	*tail;
	t_token	*current;

	head = NULL;
	tail = NULL;
	lex_init(&lex, line);
	skip_wspaces(&lex);
	while (lex.c)
	{
		current = NULL;
		if (is_char_operator(lex.c))
			current = lex_operator(&lex);
		else
			current = lex_word(&lex);
		if (!current)
			return (free_tokens(head), NULL);
		append_token(&head, &tail, current);
		skip_wspaces(&lex);
	}
	append_token(&head, &tail, make_token(END, NULL));
	return (head);
}
