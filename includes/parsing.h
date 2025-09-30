/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 22:48:29 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/29 22:18:22 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "minishell.h"
# include "parsing_helpers.h"

typedef struct s_token
{
	t_type			type;
	char			*lexeme;
	struct s_token	*next;
}					t_token;

/* Token Cursor to keep track */
typedef struct s_tokcursor
{
	t_token			*current;
}					t_tcur;

typedef struct s_lexer
{
	char			*line;
	size_t			i;
	char			c;
}					t_lexer;

typedef struct s_expand
{
	int				inquotes;
	int				qstate;
	int				argvindex;
}					t_expand;

/* Lexing */
t_token				*lexer(char *input);
t_token				*lex_word(t_lexer *lex);

void				lex_init(t_lexer *lex, char *line);
void				advance(t_lexer *lex, int n);
char				peek(t_lexer *lex);
void				skip_wspaces(t_lexer *lex);
int					is_char_operator(char c);

void				append_token(t_token **head, t_token **tail, t_token *new);
t_token				*make_token(t_type type, char *lexeme);
void				free_tokens(t_token *token);
void				cur_next(t_tcur *c);
int					is_token_redir(t_token *token);

void				syntax_error(t_token *tok, t_token *tok_head,
						t_cmd *cmd_head, t_cmd *cmd_cur);
void				lexing_error(int errcode, t_token *head);

/* Parsing */

t_cmd				*parser(t_minishell *msh);

/* Expand */
void				expand_manager(t_cmd *head, t_minishell *msh);
char				*expand_word(char *str, t_minishell *msh, int argvindex);

void				handle_dollar(t_lexer *lex, t_str_buf *sb, t_expand *exp,
						t_minishell *msh);

int					is_valid_for_key(char c);
int					is_key_valid(char *str);

void				exit_expand_fatal(t_minishell *msh);

#endif
