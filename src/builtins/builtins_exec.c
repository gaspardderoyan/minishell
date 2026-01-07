/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:40:37 by mgregoir          #+#    #+#             */
/*   Updated: 2026/01/07 17:04:57 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Opens a file with specified flags and redirects it to target fd.
** @param file: The filename to open.
** @param flags: The open flags (O_RDONLY, O_WRONLY | O_CREAT | O_TRUNC, etc.).
** @param target_fd: The file descriptor to redirect to (STDIN_FILENO or 
** STDOUT_FILENO).
** @return: 0 on success, -1 on error.
*/
static int	setup_redir(char *file, int flags, int target_fd)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	if (dup2(fd, target_fd) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/*
** Processes a single redirection based on its type.
** Handles input (<), heredoc (<<), output (>), and append (>>).
** @param cmd: The command structure (for heredoc file).
** @param cur: The current redirection to process.
** @return: 0 on success, -1 on error.
*/
static int	process_single_redir(t_cmd *cmd, t_redir *cur)
{
	int	flags;

	if (cur->type == REDIR_IN)
		return (setup_redir(cur->filename, O_RDONLY, STDIN_FILENO));
	if (cur->type == REDIR_HEREDOC)
	{
		if (cmd->heredoc_file)
			return (setup_redir(cmd->heredoc_file, O_RDONLY, STDIN_FILENO));
		return (-1);
	}
	if (cur->type == REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	return (setup_redir(cur->filename, flags, STDOUT_FILENO));
}

/*
** Applies all redirections for a builtin command.
** Iterates through the redirection list and processes each one.
** @param cmd: The command structure containing redirections.
** @return: 0 on success, -1 on error.
*/
int	apply_redirections(t_cmd *cmd)
{
	t_redir	*cur;

	cur = cmd->redirs;
	while (cur)
	{
		if (process_single_redir(cmd, cur) == -1)
			return (-1);
		cur = cur->next;
	}
	return (0);
}

/*
** Restores standard input and output to their original state.
** Uses backed-up file descriptors and closes them after restoration.
** @param data: Global data structure containing stdin/stdout backups.
*/
static void	restore_stdio(t_data *data)
{
	if (dup2(data->stdin_backup, STDIN_FILENO) == -1)
		perror("minishell: dup2");
	if (dup2(data->stdout_backup, STDOUT_FILENO) == -1)
		perror("minishell: dup2");
	close(data->stdin_backup);
	close(data->stdout_backup);
	data->stdin_backup = -1;
	data->stdout_backup = -1;
}

/*
** Executes a builtin command in the parent process.
** Saves stdin/stdout, applies redirections, executes builtin,
** then restores.
** @param cmd: The command to execute.
** @param data: Global data structure.
*/
void	execute_builtin_in_parent(t_cmd *cmd, t_data *data)
{
	data->stdin_backup = dup(STDIN_FILENO);
	data->stdout_backup = dup(STDOUT_FILENO);
	if (data->stdin_backup == -1 || data->stdout_backup == -1)
	{
		perror("minishell: dup");
		data->last_exit_code = 1;
		if (data->stdin_backup != -1)
			close(data->stdin_backup);
		if (data->stdout_backup != -1)
			close(data->stdout_backup);
		return ;
	}
	if (apply_redirections(cmd) == -1)
		data->last_exit_code = 1;
	else
		data->last_exit_code = dispatch_builtin(cmd, data);
	restore_stdio(data);
}
