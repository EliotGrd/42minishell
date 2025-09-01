/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_redir2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 08:45:55 by bsuger            #+#    #+#             */
/*   Updated: 2025/08/30 12:15:24 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pop_redir(t_redirect **top)
{
	t_redirect	*temp;

	if (*top)
	{
		temp = *top;
		(*top) = (*top)-> next;
	//	free(temp -> file);//pas free car dans mes tests pour le moment c'est en dur
		free(temp);
	}
}

void	destructor_redir(t_redirect **top)
{
	while (*top)
		pop_redir(top);
}
