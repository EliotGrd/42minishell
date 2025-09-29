/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_builder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 23:17:41 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/21 21:46:40 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	str_buf_init(t_str_buf *sb)
{
	sb->str = NULL;
	sb->len = 0;
	sb->cap = 0;
}

/**
 * @brief Put char in the struct t_str_buf so we can build a string in the struct.
 *The point is to create the string without knowing its final size with realloc
 */
int	str_buf_putc(t_str_buf *sb, char c)
{
	if (sb->len + 1 >= sb->cap)
	{
		if (sb->cap == 0)
			sb->cap = 16;
		else
			sb->cap *= 2;
		sb->str = ft_realloc(sb->str, sb->len, sb->cap * sizeof(char));
		//sb->str = realloc(sb->str, sb->cap * sizeof(char));
		if (!sb->str)
			return (ft_putendl_fd("Allocation failed, cat's sad..", 2), 0);
	}

	sb->str[sb->len] = c;
	sb->len++;
	return (1);
}

/**
 * @brief Put a string of size characters in the t_str_buf struct, it lower the 
 * number of realloc calls.
 *
 * @param size size of the string we need to append in our struct
 
void	str_buf_putn(t_str_buf *sb, char *str, size_t size)
{
	while (sb->len + size >= sb->cap)
	{
		if (sb->cap == 0)
			sb->cap = 16;
		else
			sb->cap *= 2;
		sb->str = ft_realloc(sb->str, sb->len, sb->cap);
	}
	ft_memcpy(sb->str + sb->len, str, size);
	sb->len += size;
}*/

/**
 * @brief Put a null terminated string in the t_str_buf struct
 *
 */
int	str_buf_puts(t_str_buf *sb, char *str)
{
	while (*str)
	{
		if (!str_buf_putc(sb, *str))
			return (0);
		str++;
	}
	return (1);
}

void	str_buf_free(t_str_buf *sb)
{
	ft_free((void **)&sb->str);
	sb->len = 0;
	sb->cap = 0;
}

/**
 * @brief End and null terminate the string created in t_str_buf, 
 * reset the struct so it can be reused and returns the string created.
 *
 * @return Returns the string, null terminated, created in the struct.
 */
char	*str_buf_end(t_str_buf *sb)
{
	char *str;

	if (!str_buf_putc(sb, '\0'))
		return (NULL);
	str = sb->str;
	sb->str = NULL;
	sb->len = 0;
	sb->cap = 0;
	return (str);
}
