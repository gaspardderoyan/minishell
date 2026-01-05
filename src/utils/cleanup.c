/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 14:50:13 by mgregoir          #+#    #+#             */
/*   Updated: 2026/01/05 17:01:38 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
** Removes all heredoc temporary files and frees their paths.
** Called after pipeline execution to clean up resources.
** @param data: Global data structure containing cmd_list.
*/
void	cleanup_heredocs(t_data *data)
{
	t_cmd	*cmd;

	cmd = data->cmd_list;
	while (cmd)
	{
		if (cmd->heredoc_file)
		{
			unlink(cmd->heredoc_file);
			free(cmd->heredoc_file);
			cmd->heredoc_file = NULL;
		}
		cmd = cmd->next;
	}
}

/*
** Frees all resources allocated in the child process before exiting.
** Prevents memory leaks reported by valgrind in forked processes.
** @param data: Global data structure containing all allocated resources.
*/
void	cleanup_child(t_data *data)
{
	if (data->tokens)
		token_clear(&data->tokens);
	if (data->cmd_list)
		cmd_clear(&data->cmd_list);
	if (data->line)
		free(data->line);
	if (data->env)
		ft_free_array(data->env);
	ft_lstclear(&data->env_list, free);
}

/*
** Cleans up all allocated resources before exiting the shell.
** Includes readline history and file descriptor backups.
** @param data: Global data structure containing all allocated resources.
*/
void	cleanup_exit(t_data *data)
{
	if (data->env)
		ft_free_array(data->env);
	ft_lstclear(&data->env_list, free);
	if (data->cmd_list)
		cmd_clear(&data->cmd_list);
	if (data->line)
		free(data->line);
	if (data->tokens)
		token_clear(&data->tokens);
	if (data->stdin_backup != -1)
		close(data->stdin_backup);
	if (data->stdout_backup != -1)
		close(data->stdout_backup);
}
