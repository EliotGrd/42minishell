/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   argv_builder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 19:39:24 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/21 22:01:08 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

void	argv_buf_init(t_argv_buf *avb)
{
	avb->argv = NULL;
	avb->i = 0;
	avb->cap = 0;
}

/**
 * @brief Tool to build an argv with a buffer that reallocates when 
 * more memory is needed and add a string to the newly created argv
 *
 * @param avb 
 * @param str 
 */
void	argv_buf_push(t_argv_buf *avb, char *str)
{

	if (avb->i + 1 >= avb->cap)
	{
		avb->cap += 1;
		avb->argv = ft_realloc(avb->argv, sizeof(char *) * avb->i,
				sizeof(char *) * avb->cap);
		//avb->argv = realloc(avb->argv, sizeof(char *) * avb->cap);
	}
	avb->argv[avb->i] = str;
	avb->i++;
}

/**
 * @brief Ends and null terminate the argv so it can be used
 *
 * @return Returns the built argv
 */
char	**argv_buf_end(t_argv_buf *avb)
{
	char **result;

	avb->argv = ft_realloc(avb->argv, sizeof(char *) * avb->i,
			sizeof(char *) * (avb->i + 2));
	//avb->argv = realloc(avb->argv, sizeof(char *) * (avb->i + 1));
	avb->argv[avb->i] = 0;
	result = avb->argv;
	avb->argv = NULL;
	avb->i = 0;
	avb->cap = 0;
	return (result);
}

/**
 * @brief Free each string of the argv then the global array
 *
 */
void	argv_buf_free(t_argv_buf *avb)
{
	size_t n;

	n = 0;
	while (n < avb->i)
	{
		ft_free((void **)&avb->argv[n]);
		//free(avb->argv[n]);
		//avb->argv[n] = NULL;
		n++;
	}
	ft_free((void **)avb->argv);
}
