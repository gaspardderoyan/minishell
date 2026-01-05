/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 19:49:40 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/22 18:03:26 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Searches for an environment variable value by key.
** @param env_list: The environment linked list.
** @param key: The variable name to search for (e.g., "PATH").
** @return: Pointer to the value (after '='), or NULL if not found.
*/
char	*get_env_value(t_list *env_list, char *key)
{
	size_t	len;
	char	*content;

	len = ft_strlen(key);
	while (env_list)
	{
		content = (char *)env_list->content;
		if (ft_strncmp(content, key, len) == 0 && content[len] == '=')
			return (content + len + 1);
		env_list = env_list->next;
	}
	return (NULL);
}

/*
** Finds the node containing a specific environment variable.
** @param env: The environment linked list.
** @param key: The variable name to search for.
** @return: Pointer to the node, or NULL if not found.
*/
t_list	*find_env_node(t_list *env, char *key)
{
	size_t	len;
	char	*content;

	len = ft_strlen(key);
	while (env)
	{
		content = (char *)env->content;
		if (ft_strncmp(content, key, len) == 0
			&& (content[len] == '=' || content[len] == '\0'))
			return (env);
		env = env->next;
	}
	return (NULL);
}

/*
** Creates a new env string "key=value" or just "key" if value is NULL.
** @param key: The variable name.
** @param value: The variable value (can be NULL).
** @return: The new string, or NULL on malloc failure.
*/
static char	*create_env_str(char *key, char *value)
{
	char	*temp;
	char	*new_str;

	if (value)
	{
		temp = ft_strjoin(key, "=");
		if (!temp)
			return (NULL);
		new_str = ft_strjoin(temp, value);
		free(temp);
		if (!new_str)
			return (NULL);
	}
	else
	{
		new_str = ft_strdup(key);
		if (!new_str)
			return (NULL);
	}
	return (new_str);
}

/*
** Updates an existing env variable or adds a new one.
** @param env: Pointer to the environment list.
** @param key: The variable name.
** @param value: The variable value (can be NULL for export without '=').
** @return: 0 on success, -1 on malloc failure.
*/
int	update_or_add_env(t_list **env, char *key, char *value)
{
	t_list	*node;
	t_list	*new_node;
	char	*new_str;

	new_str = create_env_str(key, value);
	if (!new_str)
		return (-1);
	node = find_env_node(*env, key);
	if (node)
	{
		free(node->content);
		node->content = new_str;
	}
	else
	{
		new_node = ft_lstnew(new_str);
		if (!new_node)
		{
			free(new_str);
			return (-1);
		}
		ft_lstadd_back(env, new_node);
	}
	return (0);
}

/*
** Removes an environment variable from the list.
** Frees the node and its content if found.
** @param env: Pointer to the environment list.
** @param key: The variable name to remove.
*/
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
		if (ft_strncmp(current->content, key, len) == 0
			&& (((char *)current->content)[len] == '='
			|| ((char *)current->content)[len] == '\0'))
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->content);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
