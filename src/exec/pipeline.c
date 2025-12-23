/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 14:56:49 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/22 18:03:01 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Parent process routine after forking a child.
** Closes used pipe ends and returns the read fd for the next iteration.
** @param cmd: The current command structure.
** @param prev_read_fd: The previous read fd to close.
** @return: The read fd for the next command, or -1 if last command.
*/
static int	parent_routine(t_cmd *cmd, int prev_read_fd)
{
	if (cmd->next)
		close(cmd->pipefd[1]);
	if (prev_read_fd != -1)
		close(prev_read_fd);
	if (cmd->next)
		return (cmd->pipefd[0]);
	return (-1);
}

/*
** Checks if command is a single builtin that must run in parent.
** Only applies to cd, export, unset, exit when alone (no pipe).
** @param cmd: The command to check.
** @param data: Global data structure.
** @return: 1 if handled in parent, 0 otherwise.
*/
static int	exec_solo_builtin(t_cmd *cmd, t_data *data)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (0);
	if (cmd->next != NULL)
		return (0);
	if (!is_modifier_builtin(cmd->args[0]))
		return (0);
	execute_builtin_in_parent(cmd, data);
	return (1);
}

/*
** Iterates through commands, creates pipes and forks children.
** @param data: Global data structure.
** @param cmd: First command in the pipeline.
*/
static void	fork_pipeline(t_data *data, t_cmd *cmd)
{
	int	prev_fd;

	prev_fd = -1;
	while (cmd)
	{
		if (!set_pipe(cmd))
			return ;
		cmd->pid = fork();
		if (cmd->pid == -1)
		{
			perror("fork");
			if (prev_fd != -1)
				close(prev_fd);
			break ;
		}
		if (cmd->pid == 0)
			exec_child(cmd, data, prev_fd);
		prev_fd = parent_routine(cmd, prev_fd);
		cmd = cmd->next;
	}
}

/*
** Main pipeline execution function.
** Handles heredocs, solo builtins, then forks and waits for children.
** @param data: Global data structure containing the command list.
*/
void	execute_pipeline(t_data *data)
{
	if (check_heredoc(data) != 0)
		return ;
	if (exec_solo_builtin(data->cmd_list, data))
	{
		cleanup_heredocs(data);
		return ;
	}
	fork_pipeline(data, data->cmd_list);
	wait_all_children(data);
	set_signal_action();
	cleanup_heredocs(data);
}
