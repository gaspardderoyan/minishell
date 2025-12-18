/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 18:03:06 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/18 18:03:07 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Prints an error message and exits the child process cleanly.
** Frees allocated resources before exiting.
** @param cmd: The current command (to display its name).
** @param data: Global data structure (to free env).
** @param msg: The error message to display.
** @param err_code: Exit code (127 = not found, 126 = permission denied).
*/
static void	error_exit(t_cmd *cmd, t_data *data, char *msg, int err_code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd->args && cmd->args[0])
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_lstclear(&data->env_list, free);
	exit(err_code);
}

/*
** Prepares and executes the command with execve().
** Resolves the command path, converts the environment to an array,
** then replaces the process with the command to execute.
** @param cmd: The command to execute.
** @param data: Global data structure containing the environment.
*/
static void	do_execve(t_cmd *cmd, t_data *data)
{
	char    **env_array;

	if (!cmd->args || !cmd->args[0])
	{
		//ft_free_array(data->env);
		ft_lstclear(&data->env_list, free);
		exit(0);
	}
	init_cmd_path(cmd, data);
	if (!cmd->cmd_path)
		error_exit(cmd, data, ": command not found\n", 127);
	env_array = env_list_to_array(data->env_list);
	if (!env_array)
		error_exit(cmd, data, ": malloc error\n", 1);
	if (execve(cmd->cmd_path, cmd->args, env_array) == -1)
	{
		perror("minishell");
		ft_free_array(env_array);
		ft_lstclear(&data->env_list, free);
		exit(126);
	}
}

/*
** Entry point of the child process after fork().
** Sets up pipes, applies redirections, then executes the command.
** @param cmd: The command to execute.
** @param data: Global data structure.
** @param prev_read_fd: Read fd from previous pipe (-1 if first command).
*/
void	exec_child(t_cmd *cmd, t_data *data, int prev_read_fd)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	connect_pipes(cmd, prev_read_fd);
	handle_redir_fds(cmd);
	close_all_pipes(data->cmd_list);
	do_execve(cmd, data);
}
