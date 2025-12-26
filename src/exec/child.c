/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 18:03:06 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/26 16:03:04 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Prints an error message and exits the child process cleanly.
** Frees allocated resources before exiting.
** @param cmd: The current command (to display its name).
** @param data: Global data structure (to free env).
** @param msg: The error message to display.
** @param err_code: Exit code (EXIT_CMD_NOT_FOUND or EXIT_CANT_EXEC).
*/
static void	error_exit(t_cmd *cmd, t_data *data, char *msg, int err_code)
{
	char	*cmd_name;

	cmd_name = NULL;
	if (cmd->args && cmd->args[0])
		cmd_name = cmd->args[0];
	print_error(cmd_name, NULL, msg);
	cleanup_child(data);
	exit(err_code);
}

/*
** Checks if the command path points to a directory.
** If so, prints "Is a directory" and exits with EXIT_CANT_EXEC.
** @param cmd: The command structure.
** @param data: Global data structure.
*/
static void	check_directory(t_cmd *cmd, t_data *data)
{
	struct stat	sb;

	if (stat(cmd->cmd_path, &sb) == 0 && S_ISDIR(sb.st_mode))
		error_exit(cmd, data, ": Is a directory\n", CANT_EXEC);
}

/*
** Handles execve error based on errno value.
** @param env_array: Environment array to free.
** @param cmd: The command structure.
** @param data: Global data structure.
*/
static void	handle_execve_error(char **env_array, t_cmd *cmd, t_data *data)
{
	ft_free_array(env_array);
	if (errno == EACCES)
		error_exit(cmd, data, ": Permission denied\n", CANT_EXEC);
	else if (errno == ENOENT)
		error_exit(cmd, data, ": No such file or directory\n", CMD_NOT_FOUND);
	else
	{
		perror("minishell");
		cleanup_child(data);
		exit(CANT_EXEC);
	}
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
	char	**env_array;

	if (!cmd->args || !cmd->args[0])
	{
		cleanup_child(data);
		exit(0);
	}
	init_cmd_path(cmd, data);
	if (!cmd->cmd_path)
		error_exit(cmd, data, ": command not found\n", CMD_NOT_FOUND);
	check_directory(cmd, data);
	env_array = env_list_to_array(data->env_list);
	if (!env_array)
		error_exit(cmd, data, ": malloc error\n", 1);
	if (execve(cmd->cmd_path, cmd->args, env_array) == -1)
		handle_execve_error(env_array, cmd, data);
}

/*
** Entry point of the child process after fork().
** Resets signals, connects pipes, applies redirections, then executes command.
** Handles both builtins (in child) and external commands.
** @param cmd: The command to execute.
** @param data: Global data structure.
** @param prev_read_fd: Read fd from previous pipe (-1 if first command).
*/
void	exec_child(t_cmd *cmd, t_data *data, int prev_read_fd)
{
	int	exit_code;

	reset_signals_default();
	connect_pipes(cmd, prev_read_fd);
	handle_redir_fds(cmd);
	close_all_pipes(data->cmd_list);
	if (cmd->args && is_builtin(cmd->args[0]))
	{
		exit_code = dispatch_builtin(cmd, data);
		cleanup_child(data);
		exit(exit_code);
	}
	do_execve(cmd, data);
}
