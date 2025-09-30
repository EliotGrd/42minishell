/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helpers.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 19:45:08 by egiraud           #+#    #+#             */
/*   Updated: 2025/08/27 23:00:04 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HELPERS_H
# define PARSING_HELPERS_H

# include "minishell.h"
# include "parsing.h"

/* Argv Builder */
typedef struct s_argv_buf
{
	char	**argv;
	size_t	i;
	size_t	cap;
}			t_argv_buf;

void		argv_buf_init(t_argv_buf *avb);
int			argv_buf_push(t_argv_buf *avb, char *str);
char		**argv_buf_end(t_argv_buf *avb);
void		argv_buf_free(t_argv_buf *avb);

/* String Builder */
typedef struct s_str_buf
{
	char	*str;
	size_t	len;
	size_t	cap;
}			t_str_buf;

void		str_buf_init(t_str_buf *sb);
int			str_buf_putc(t_str_buf *sb, char c);
int			str_buf_puts(t_str_buf *sb, char *str);
char		*str_buf_end(t_str_buf *sb);
void		str_buf_free(t_str_buf *sb);

#endif
