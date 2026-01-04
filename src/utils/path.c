/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 12:34:07 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/24 15:36:55 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Iterates through PATH directories searching for a matching executable.
** Concatenates each path with the command name and checks access.
** @param paths: Array of PATH directories (split by ':').
** @param cmd: The command name to search for.
** @return: The full path if found (must be freed), or NULL otherwise.
*/
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

/*
** Resolves the full path of a command.
** If cmd contains '/', treats it as a direct path.
** Otherwise, searches through PATH directories.
** @param cmd: The command name or path.
** @param env_list: The environment linked list.
** @return: The full executable path (must be freed), or NULL if not found.
*/
char	*get_full_path(char *cmd, t_list *env_list)
{
	char	**paths;
	char	*path_env;
	char	*full_path;

	if (cmd && (ft_strchr(cmd, '/') != NULL))
		return (ft_strdup(cmd));
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
** Prepares the command path for execution.
** Calls get_full_path and stores the result in cmd->cmd_path.
** @param cmd: The command structure to initialize.
** @param data: Global data structure containing the environment.
*/
void	init_cmd_path(t_cmd *cmd, t_data *data)
{
	if (!cmd->args || !cmd->args[0])
		return ;
	cmd->cmd_path = get_full_path(cmd->args[0], data->env_list);
}
