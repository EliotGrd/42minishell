/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:17:31 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/11 15:02:28 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(t_env *top_env, char **str)
{
	if (str[1] != NULL)
	{
		ft_putstr_fd("Too much args\n", 2);
		return (1);
	}
	while (top_env)
	{
		ft_printf("%s=%s\n", top_env -> key, top_env -> value);
		top_env = top_env -> next;
	}
	return (0);
}
