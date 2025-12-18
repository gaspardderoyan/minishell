/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 14:56:49 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/16 18:36:56 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Waits for all child processes to terminate.
** Stores the exit code of the last command in data->last_exit_code.
** @param data: Global data structure containing the command list.
*/
static void	wait_all_children(t_data *data)
{
	t_cmd	*cmd;
	int		status;

	cmd = data->cmd_list;
	reset_signals();
	while (cmd)
	{
		if (cmd->pid != -1)
		{
			waitpid(cmd->pid, &status, 0);
			if (cmd->next == NULL)
			{
				if (WIFEXITED(status))
					data->last_exit_code = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
				{
					data->last_exit_code = 128 + WTERMSIG(status);
					if (WTERMSIG(status) == SIGQUIT)
						ft_putstr_fd("Quit (core dumped)\n", 1);
					if (WTERMSIG(status) == SIGINT)
						ft_putstr_fd("\n", 1);
				}
			}
		}
		cmd = cmd->next;
	}
}

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
** Main pipeline execution function.
** Iterates through commands, creates pipes, forks children, and waits.
** @param data: Global data structure containing the command list.
*/
void	execute_pipeline(t_data *data)
{
	t_cmd	*cmd;
	int		prev_fd;

	cmd = data->cmd_list;
	//if (exec_solo_builtin(cmd, data))
	//	return ;
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
	wait_all_children(data);
	set_signal_action();
}
