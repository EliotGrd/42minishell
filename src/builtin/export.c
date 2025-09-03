/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/01 09:33:38 by bsuger            #+#    #+#             */
/*   Updated: 2025/09/03 13:32:12 by bsuger           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env *research_node_env(t_env *top_env, char *search)
{
	while (top_env)
	{
		if (ft_strncmp(search, top_env -> key, ft_strlen(search) + 1) == 0)
			return (top_env);
		top_env = top_env -> next;
	}
	return (NULL);
}

int	export(char **str, t_env *top_env)
{
	t_env	*temporary;
	char	*extract_key;
	char	*extract_value;

	env(top_env);//temporaire pour voir
	if (str[1] == NULL || ft_strchr(str[1], '=') == NULL)
		return (1);
	extract_key = ft_strtok(str[1], "=");
	extract_value = ft_strtok(NULL, "=");//c'est ca qu'il faut expand et de-quotes
	ft_printf("extract key %s\nextract value %s\n", extract_key, extract_value);//temp
	temporary = research_node_env(top_env, str[1]);
	if (temporary == NULL)//il a pas trouve faut cree un noeud et l'add
	{
		extract_key = ft_strdup(extract_key);
		extract_value = ft_strdup(extract_value);
		//securite des 2 en meme temps avec ft_free a ajouter comme ca serat mieux
		temporary = create_node_env(extract_key, extract_value);
		//safety surmement ici et bien tester avec un env vide mais normalement c'est good
		push_back_env(&top_env, temporary);
	}
	else //il y a trouve un noeud il faut l'update
	{
		extract_value = ft_strdup(extract_value);
 		//safety a faire du ft_strdup => est ce qu'on termine le terminal ? ou juste j'ajoute "" ?
		free(temporary -> value);
		temporary -> value = extract_value;
	}
	ft_printf("-------------------------------------------\n");//temporaire pour les tests
	env(top_env);//temporaire pour voir si ca ajoute la valeur.
	return (0);
}

