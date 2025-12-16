/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:12:46 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/16 20:55:04 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Initializes a linked list from the environment array.
** Each environment variable is duplicated into a t_list node.
** @param env: The environment array (char **envp from main).
** @return: The environment linked list, or NULL on error.
*/
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
		{
			ft_lstclear(&env_list, free);
			return (NULL);
		}
		new_node = ft_lstnew(content);
		if (!new_node)
		{
			free(content);
			ft_lstclear(&env_list, free);
			return (NULL);
		}
		ft_lstadd_back(&env_list, new_node);
		i++;
	}
	return (env_list);
}

/*
** Converts the environment linked list to a string array.
** Required to pass the environment to execve().
** @param env_list: The environment linked list.
** @return: A NULL-terminated string array, or NULL on error.
*/
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
