/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 12:34:07 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/10 14:40:01 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_env_value(t_list *env_list, char *key)
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

static char	*search_in_paths(char **paths, char *cmd)
{
	char	*temp;
	char	*path;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		path = ft_strjoin(temp, cmd);
		free(temp);
		if (!path)
			return (NULL);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

char	*get_full_path(char *cmd, t_list *env_list)
{
	char	**paths;
	char	*path_env;
	char	*full_path;

	if (cmd && (ft_strchr(cmd, '/') != NULL))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	if (!cmd || !*cmd || !env_list)
		return (NULL);
	path_env = get_env_value(env_list, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full_path = search_in_paths(paths, cmd);
	ft_free_array(paths);
	return (full_path);
}
