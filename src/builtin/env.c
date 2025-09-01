/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 14:17:31 by bsuger            #+#    #+#             */
/*   Updated: 2025/07/28 14:17:34 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env(t_env *top_env)
{
	while (top_env)
	{
		ft_printf("%s=%s\n", top_env -> key, top_env -> value);
		top_env = top_env -> next;
	}
}
