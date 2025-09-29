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

static void	fall_back(char **to_free, size_t max)
{
	size_t	i;

	i = 0;
	while (i < max)
	{
		ft_free((void **)&to_free[i]);
		i++;
	}
	ft_free((void **)&to_free);
}

/**
 * @brief Tool to build an argv with a buffer that reallocates when
 * more memory is needed and add a string to the newly created argv
 *
 * @param avb
 * @param str
 */
int	argv_buf_push(t_argv_buf *avb, char *str)
{
	char	**tmp;

	if (avb->i + 1 >= avb->cap)
	{
		avb->cap += 1;
		tmp = avb->argv;
		avb->argv = ft_realloc(avb->argv, sizeof(char *) * avb->i,
				sizeof(char *) * avb->cap);
		if (!avb->argv)
		{
			ft_putendl_fd(MALLOC_ERR, 2);
			fall_back(tmp, avb->i);
			ft_free((void **)&str);
			return (0);
		}
		tmp = NULL;
	}
	avb->argv[avb->i] = str;
	avb->i++;
	return (1);
}

/**
 * @brief Ends and null terminate the argv so it can be used
 *
 * @return Returns the built argv
 */
char	**argv_buf_end(t_argv_buf *avb)
{
	char	**result;
	char	**tmp;

	tmp = avb->argv;
	avb->argv = ft_realloc(avb->argv, sizeof(char *) * avb->i, sizeof(char *)
			* (avb->i + 1));
	if (!avb->argv)
		return (ft_putendl_fd(MALLOC_ERR, 2), fall_back(tmp, avb->i), NULL);
	avb->argv[avb->i] = 0;
	result = avb->argv;
	tmp = NULL;
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
	size_t	n;

	n = 0;
	if (!avb->argv)
		return ;
	while (n < avb->i)
	{
		ft_free((void **)&avb->argv[n]);
		n++;
	}
	ft_free((void **)&avb->argv);
}
