/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:40:37 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/23 18:19:57 by mgregoir         ###   ########.fr       */
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
	dup2(fd, target_fd);
	close(fd);
	return (0);
}

/*
** Applies all redirections for a builtin command.
** @param cmd: The command structure containing redirections.
** @return: 0 on success, -1 on error.
*/
int	apply_redirections(t_cmd *cmd)
{
	t_redir	*cur;
	int		flags;
	int		ret;

	cur = cmd->redirs;
	while (cur)
	{
		if (cur->type == REDIR_IN || cur->type == REDIR_HEREDOC)
			ret = setup_redir(cur->filename, O_RDONLY, STDIN_FILENO);
		else if (cur->type == REDIR_OUT)
		{
			flags = O_WRONLY | O_CREAT | O_TRUNC;
			ret = setup_redir(cur->filename, flags, STDOUT_FILENO);
		}
		else
		{
			flags = O_WRONLY | O_CREAT | O_APPEND;
			ret = setup_redir(cur->filename, flags, STDOUT_FILENO);
		}
		if (ret == -1)
			return (-1);
		cur = cur->next;
	}
	return (0);
}

/*
** Executes a builtin command in the parent process.
** Saves and restores stdin/stdout around redirections.
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
	dup2(data->stdin_backup, STDIN_FILENO);
	dup2(data->stdout_backup, STDOUT_FILENO);
	close(data->stdin_backup);
	close(data->stdout_backup);
	data->stdin_backup = -1;
	data->stdout_backup = -1;
}
