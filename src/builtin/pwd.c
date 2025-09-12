/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 08:22:12 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/12 14:50:50 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(void)
{
	int		i;
	char	*buf;

	i = 30;
	buf = malloc(i);
	if (!buf)
		return (1);
	while (getcwd(buf, i) == NULL && errno == ERANGE)
	{
		free(buf);
		buf = NULL;
		i *= 10;
		buf = malloc(i);
		if (!buf)
			return (1);
		getcwd(buf, i);
	}
	ft_printf("%s\n", buf);
	free(buf);
	return (0);
}
