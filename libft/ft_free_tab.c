/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_tab.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 15:25:25 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/08 15:46:52 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_free_tab(char **result)
{
	int	i;

	i = 0;
	while (result[i] != NULL)
	{
		ft_free((void **)&result[i]);
		i++;
	}
	ft_free((void **)&result);
}
