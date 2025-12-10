/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:12:46 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/10 18:28:13 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list	*init_env_list(char **env)
{
	t_list	*env_list;
	t_list	*new_node;
	char	*content;
	int		i;

	env_list = NULL;
	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		content = ft_strdup(env[i]);
		if (!content)
			return (NULL);
		new_node = ft_lstnew(content);
		if (!new_node)
		{
			free(content);
			return (NULL);
		}
		ft_lstadd_back(&env_list, new_node);
		i++;
	}
	return (env_list);
}

char	**env_list_to_array(t_list *env_list)
{
	char	**env_array;
	int		count;
	int		i;

	count = ft_lstsize(env_list);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env_list)
	{
		if (env_list->content)
		{
			env_array[i] = ft_strdup((char *)env_list->content);
			if (!env_array[i])
			{
				ft_free_array(env_array);
				return (NULL);
			}
		}
		env_list = env_list->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}
