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

void	close_all_pipes(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	tmp = cmd_list;
	while (tmp)
	{
		// Fermeture du côté LECTURE
		if (tmp->pipefd[0] != -1)
		{
			close(tmp->pipefd[0]);
			tmp->pipefd[0] = -1; // Sécurité pour éviter double close
		}
		// Fermeture du côté ÉCRITURE
		if (tmp->pipefd[1] != -1)
		{
			close(tmp->pipefd[1]);
			tmp->pipefd[1] = -1;
		}
		tmp = tmp->next;
	}
}

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
