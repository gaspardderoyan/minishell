/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:05:07 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/18 19:32:41 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Gets the value of an environment variable from the list.
** @param env_list: The environment linked list.
** @param key: The variable name to search for.
** @return: Pointer to the value (after '='), or NULL if not found.
*/
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

/*
** Gets the target path for cd command.
** Handles: no arg (HOME), "-" (OLDPWD), or the given path.
** @param args: The argument array.
** @param data: Global data structure.
** @return: The target path, or NULL on error.
*/
static char	*get_cd_path(char **args, t_data *data)
{
	char	*path;

	if (!args[1])
	{
		path = get_env_value(data->env_list, "HOME");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			return (NULL);
		}
		return (path);
	}
	if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_env_value(data->env_list, "OLDPWD");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: OLDPWD not set", STDERR_FILENO);
			return (NULL);
		}
		ft_putendl_fd(path, STDOUT_FILENO);
		return (path);
	}
	return (args[1]);
}

/*
** Builtin cd command.
** Changes the current directory and updates PWD/OLDPWD.
** @param args: The argument array.
** @param data: Global data structure.
** @return: 0 on success, 1 on error.
*/
int	builtin_cd(char **args, t_data *data)
{
	char	old_cwd[PATH_MAX];
	char	new_cwd[PATH_MAX];
	char	*path;

	path = get_cd_path(args, data);
	if (!path)
		return (1);
	if (!getcwd(old_cwd, sizeof(old_cwd)))
		old_cwd[0] = '\0';
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
		perror(path);
		return (1);
	}
	update_or_add_env(&data->env_list, "OLDPWD", old_cwd);
	if (getcwd(new_cwd, sizeof(new_cwd)))
		update_or_add_env(&data->env_list, "PWD", new_cwd);
	return (0);
}
