/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   my_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/29 14:07:38 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/13 11:07:08 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_option_echo(char *str)
{
	++str;
	while(*str)
	{
		if (*str != 'n' && *str != 'e')
			return (0);
		str++;
	}
	return (1);
}

int	my_echo(char **argv)
{
	int	i;
	int	n;

	i = 0;
	n = 0;
	while (argv[i][0] == '-' && check_option_echo(argv[i]))
	{
		n = 1;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (n == 0)
		write(1, "\n", 1);
	return (0);
}
