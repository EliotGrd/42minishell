/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 19:24:09 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/02 16:44:41 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
 #include "../../includes/parsing.h"

/**
 * @brief Build a token
 *
 * @param type 
 * @param lexeme 
 * @param q_state 
 */
t_token	*make_token(t_type type, char *lexeme, unsigned int q_state)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	if (!token)
		return (NULL); // flag error
	token->type = type;
	token->q_state = q_state;
	if (lexeme)
		token->lexeme = ft_strdup(lexeme);
	return (token);
}

/**
 * @brief Method of append is a linked list with pointers
 *
 * @param head previous token
 * @param tail next token
 * @param new token to append
 */
void	append_token(t_token **head, t_token **tail, t_token *new)
{
	if (!new)
		return;
	if (!*head)
	{
		*tail = new;
		*head = *tail;
	}
	else 
	{
		(*tail)->next = new;
		*tail = new;
	}
}

/**
 * @brief Free t_token entire linked list
 *
 * @param token head of stack
 */
void	free_tokens(t_token *token)
{
	t_token *temp;

	while (token)
	{
		temp = token->next;
		ft_free((void **)&token->lexeme);
		//ft_printf("%s, %p\n", token->lexeme, &token->lexeme);
		//free(token->lexeme);
		ft_free((void **)&token);
		//ft_printf("%p\n", &token);
		//free(token);
		token = temp;
	}
}
