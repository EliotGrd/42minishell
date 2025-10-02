/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/30 11:08:46 by bsuger            #+#    #+#             */
/*   Updated: 2025/10/02 08:56:55 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_pwd(char *key, t_minishell *minishell)
{
	int		i;
	t_env	*temp;
	char	*buf;

	i = 30;
	temp = research_node_env(minishell -> top_env, key);
	if (!temp)
		return ;
	buf = malloc(i);
	if (!buf)
		return ;
	while (getcwd(buf, i) == NULL && errno == ERANGE)
	{
		free(buf);
		buf = NULL;
		i *= 10;
		buf = malloc(i);
		if (!buf)
			return (ft_putendl_fd(MALLOC_ERR, 2));
		getcwd(buf, i);
	}
	free(temp -> value);
	temp -> value = buf;
}

int	len(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		i++;
	return (i);
}

int	intermediary_cd(t_minishell *minishell)
{
	char	*home;

	home = research_key_env(minishell -> top_env, "HOME");
	if (home == NULL)
		return (ft_putstr_fd("HOME is not set\n", 2), 1);
	if (home[0] == '\0')
		return (free(home), 0);
	if (chdir(home) == -1)
		return (perror("CHAT$ "), free(home), 1);
	free(home);
	return (1);
}

int	cd(char **argv, t_minishell *minishell)
{
	if (len(argv + 1) > 1)
		return (ft_putstr_fd("Too much arguments\n", 2), 1);
	update_pwd("OLDPWD", minishell);
	if (argv[1] == NULL)
		return (intermediary_cd(minishell));
	else
	{
		if (*argv[1] == '\0')
			return (0);
		if (chdir(argv[1]) == -1)
			return (perror("CHAT$ "), 1);
	}
	update_pwd("PWD", minishell);
	return (0);
}
