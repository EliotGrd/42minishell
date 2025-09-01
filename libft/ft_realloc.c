/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 16:46:52 by egiraud           #+#    #+#             */
/*   Updated: 2025/08/22 16:47:18 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_realloc(void *to_realloc, size_t old_size, size_t new_size)
{
	void	*new_ptr;
	size_t	tocopy;

	if (to_realloc == NULL)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(to_realloc);
		return (malloc(0));
	}
	new_ptr = malloc(new_size);
	if (new_size > old_size)
		tocopy = new_size;
	else
		tocopy = old_size;
	ft_memcpy(new_ptr, to_realloc, tocopy);
	free(to_realloc);
	return (new_ptr);
}
