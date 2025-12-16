/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 14:50:13 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/16 18:38:03 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Closes all pipe file descriptors in the command list.
** Sets fd to -1 after closing to prevent double-close errors.
** @param cmd_list: The head of the command linked list.
*/
void	close_all_pipes(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	tmp = cmd_list;
	while (tmp)
	{
		if (tmp->pipefd[0] != -1)
		{
			close(tmp->pipefd[0]);
			tmp->pipefd[0] = -1;
		}
		if (tmp->pipefd[1] != -1)
		{
			close(tmp->pipefd[1]);
			tmp->pipefd[1] = -1;
		}
		tmp = tmp->next;
	}
}

/*
** Closes the input and output file descriptors of a command.
** Only closes if fd is valid and not stdin/stdout.
** @param cmd: The command structure containing fd_in and fd_out.
*/
void	close_cmd_fds(t_cmd *cmd)
{
	if (cmd->fd_in != -1 && cmd->fd_in != STDIN_FILENO)
	{
		close(cmd->fd_in);
		cmd->fd_in = -1;
	}
	if (cmd->fd_out != -1 && cmd->fd_out != STDOUT_FILENO)
	{
		close(cmd->fd_out);
		cmd->fd_out = -1;
	}
}
