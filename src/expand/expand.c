/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: egiraud <egiraud@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 16:36:33 by egiraud           #+#    #+#             */
/*   Updated: 2025/09/21 21:48:22 by egiraud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/parsing.h"

/**
 * @brief Checks if a given char is a valid character for the key of 
 * an env variable
 *
 * @return Returns 1 if char valid, 0 if invalid
 */
int	is_valid_for_key(char c)
{
	if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90) || (c >= 48 && c <= 57)
		|| c == 95)
		return (1);
	return (0);
}

/* 
int	is_key_valid(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (str[0] >= 48 && str[0] <= 57)
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!is_valid_for_key(str[i]))
			return (0);
		i++;
	}
	return (1);
}*/

/**
 * @brief Removes a given index of an argv in case of env variable not found
 *
 * @param argv cmd arguments that need a removal
 * @param msh 
 */
static void	argv_remove_index(char **argv, t_minishell *msh)
{
	int	i;

	if (!argv)
		return ;
	if (argv[msh->index_rm_exp][0] != '$' && argv[msh->index_rm_exp][0])
	{
		msh->index_rm_exp = -1;
		return ;
	}
	ft_free((void **)&argv[msh->index_rm_exp]);
	i = msh->index_rm_exp + 1;
	argv[msh->index_rm_exp] = argv[i];
	while (argv[i])
	{
		argv[i] = argv[i + 1];
		i++;
	}
	msh->index_rm_exp = -1;
}

/**
 * @brief Iterate over every args and redir of each command to expand 
 * and dequote what needs to be
 *
 */
void	expand_manager(t_cmd *head, t_minishell *msh)
{
	t_cmd		*cur;
	t_redirect	*cur_redir;
	int			i;

	cur = head;
	while (cur)
	{
		i = 0;
		while (cur->args && cur->args[i])
		{
			cur->args[i] = expand_word(cur->args[i], msh, i);
			if (msh->index_rm_exp > -1)
				argv_remove_index(cur->args, msh);
			i++;
		}
		cur_redir = cur->redir;
		while (cur_redir)
		{
			cur_redir->file = expand_word(cur_redir->file, msh, 0);
			if (msh->index_rm_exp > -1)
				exp_redir_error(msh, *cur_redir);
			cur_redir = cur_redir->next;
		}
		cur = cur->next;
	}
}
