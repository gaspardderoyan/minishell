/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_dispatch.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:39:47 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/22 18:03:36 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (127);
}
