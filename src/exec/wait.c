/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 19:00:00 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/22 19:00:00 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Handles the exit status of a terminated child process.
** Sets the exit code and prints signal messages if needed.
** @param data: Global data structure to store exit code.
** @param status: The status returned by waitpid.
*/
static void	handle_child_status(t_data *data, int status)
{
	if (WIFEXITED(status))
		data->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		data->last_exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGQUIT)
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
		if (WTERMSIG(status) == SIGINT)
			printf("\n");
	}
}

/*
** Waits for all child processes to terminate.
** Stores the exit code of the last command in data->last_exit_code.
** @param data: Global data structure containing the command list.
*/
void	wait_all_children(t_data *data)
{
	t_cmd	*cmd;
	int		status;

	cmd = data->cmd_list;
	ignore_signals();
	while (cmd)
	{
		if (cmd->pid != -1)
		{
			waitpid(cmd->pid, &status, 0);
			if (cmd->next == NULL)
				handle_child_status(data, status);
		}
		cmd = cmd->next;
	}
}
