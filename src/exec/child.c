/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 18:03:06 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/23 12:35:35 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char	**env_array;

	if (!cmd->args || !cmd->args[0])
	{
		ft_free_array(data->env);
		data->env = NULL;
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
** Resets signal handlers to default behavior (SIG_DFL).
** Called in child processes to restore normal signal handling 
** for SIGINT/SIGQUIT.
*/
static void	reset_signals_default(void)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
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
		ft_lstclear(&data->env_list, free);
		exit(exit_code);
	}
	do_execve(cmd, data);
}
