/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 12:34:07 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/16 20:55:10 by mgregoir         ###   ########.fr       */
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
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
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

/*
    Prépare le chemin de la commande.
    Appelle find_path et stocke le résultat dans cmd->cmd_path.
*/
void	init_cmd_path(t_cmd *cmd, t_data *data)
{
	// Sécurité : Si pas d'arguments (ex: redirection seule "< file"), on ne fait rien
	if (!cmd->args || !cmd->args[0])
		return ;

	// On appelle la fonction de recherche qu'on a codée en Phase 1
	// Elle retourne le chemin absolu (ex: "/bin/ls") ou NULL
	cmd->cmd_path = get_full_path(cmd->args[0], data->env_list);
}
