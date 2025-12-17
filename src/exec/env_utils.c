/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 19:49:40 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/17 20:30:08 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_list	*find_env_node(t_list *env, char *key)
{
	size_t	len;
	char	*content;

	len = ft_strlen(key);
	while (env)
	{
		content = (char *)env->content;
		if (ft_strncmp(content, key, len) == 0 && 
			(content[len] == '=' || content[len] == '\0'))
			return (env);
		env = env->next;
	}
	return (NULL);
}

void	update_or_add_env(t_list **env, char *key, char *value)
{
	t_list	*node;
	char	*temp;
	char	*new_str;

	if (value)
	{
		temp = ft_strjoin(key, "=");
		new_str = ft_strjoin(temp, value);
		free(temp);
	}
	else
		new_str = ft_strdup(key);
	node = find_env_node(*env, key);
	if (node)
	{
		free(node->content);
		node->content = new_str;
	}
	else
		ft_lstadd_back(env, ft_lstnew(new_str));
}

void	remove_env_node(t_list **env, char *key)
{
	t_list	*current;
	t_list	*prev;
	size_t	len;

	current = *env;
	prev = NULL;
	len = ft_strlen(key);
	while (current)
	{
		if (ft_strncmp(current->content, key, len) == 0 &&
			(((char *)current->content)[len] == '='
			|| ((char *)current->content)[len] == '\0'))
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next; // On supprime le premier
			free(current->content);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
