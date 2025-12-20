/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:40:37 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/19 16:51:49 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		perror("minishell: dup");
		data->last_exit_code = 1;
		if (saved_stdin != -1)
			close(saved_stdin);
		if (saved_stdout != -1)
			close(saved_stdout);
		return ;
	}
	if (apply_redirections(cmd) == -1)
		data->last_exit_code = 1;
	else
		data->last_exit_code = dispatch_builtin(cmd, data);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
