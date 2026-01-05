/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatch.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:39:47 by mgregoir          #+#    #+#             */
/*   Updated: 2026/01/05 12:09:36 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Checks if a command name is a builtin.
** @param cmd: The command name to check.
** @return: 1 if builtin, 0 otherwise.
*/
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

/*
** Checks if a builtin modifies the parent shell environment.
** Modifier builtins: cd, export, unset, exit.
** @param cmd: The command name to check.
** @return: 1 if modifier builtin, 0 otherwise.
*/
int	is_modifier_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

/*
** Dispatches execution to the appropriate builtin function.
** @param cmd: The command structure containing args.
** @param data: Global data structure.
** @return: The exit code of the builtin, or 
** EXIT_CMD_NOT_FOUND if not found.
*/
int	dispatch_builtin(t_cmd *cmd, t_data *data)
{
	char	*name;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	name = cmd->args[0];
	if (ft_strncmp(name, "echo", 5) == 0)
		return (builtin_echo(cmd->args));
	if (ft_strncmp(name, "cd", 3) == 0)
		return (builtin_cd(cmd->args, data));
	if (ft_strncmp(name, "pwd", 4) == 0)
		return (builtin_pwd());
	if (ft_strncmp(name, "export", 7) == 0)
		return (builtin_export(cmd->args, data));
	if (ft_strncmp(name, "unset", 6) == 0)
		return (builtin_unset(cmd->args, data));
	if (ft_strncmp(name, "env", 4) == 0)
		return (builtin_env(data->env_list));
	if (ft_strncmp(name, "exit", 5) == 0)
		return (builtin_exit(cmd->args, data));
	return (CMD_NOT_FOUND);
}
