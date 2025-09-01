/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 23:32:36 by egiraud           #+#    #+#             */
/*   Updated: 2025/08/23 17:18:25 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

/**
 * @brief Initialize t_lexer
 */
void	lex_init(t_lexer *lex, char *line)
{
	lex->line = line;
	lex->i = 0;
	lex->c = line[0];
}

/**
 * @brief Advance one byte in the struct t_lexer to keep track of progress
 */
void	advance(t_lexer *lex, int n)
{
	while (lex->c && n > 0)
	{
		lex->i++;
		lex->c = lex->line[lex->i];
		n--;
	}
}

/**
 * @brief "Peek", Look at the next character in the input string
 *
 * @return The char looked up is returned
 */
char	peek(t_lexer *lex)
{
	return (lex->line[lex->i + 1]);
}

/**
 * @brief Skip whitespaces in the t_lexer struct
 */
void	skip_wspaces(t_lexer *lex)
{
	while (lex->c && ft_iswspace(lex->c))
	{
		advance(lex, 1);
	}
}
