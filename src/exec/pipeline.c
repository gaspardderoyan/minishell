/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 14:56:49 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/16 16:20:30 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	wait_all_children(t_data *data)
{
	t_cmd	*cmd;
	int		status;

	cmd = data->cmd_list;
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
					data->last_exit_code = 128 + WTERMSIG(status);
			}
		}
		cmd = cmd->next;
	}
}

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
    Vérifie si on a affaire à un Builtin Solo (cd, export...).
    Si oui, l'exécute et retourne 1. Sinon retourne 0.
*/
static int	exec_solo_builtin(t_cmd *cmd, t_data *data)
{
	if (cmd && !cmd->next && cmd->args && is_modifier_builtin(cmd->args[0]))
	{
		execute_builtin_in_parent(cmd, data);
		return (1);
	}
	return (0);
}

void	execute_pipeline(t_data *data)
{
	t_cmd	*cmd;
	int		prev_fd;

	cmd = data->cmd_list;
	if (exec_solo_builtin(cmd, data))
		return ;
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
}
