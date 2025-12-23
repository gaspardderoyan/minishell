/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:43:41 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/22 18:03:06 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Creates a pipe for communication between the current and next command.
** Does nothing if there is no next command in the pipeline.
** @param cmd: The current command structure.
** @return: 1 on success, 0 on failure.
*/
int	set_pipe(t_cmd *cmd)
{
	if (!cmd->next)
		return (1);
	if (pipe(cmd->pipefd) == -1)
	{
		perror("minishell: pipe");
		return (0);
	}
	return (1);
}

/*
** Opens a file for redirection based on the redirection type.
** @param redir: The redirection structure containing type and filename.
** @return: The file descriptor, or -1 on error.
*/
static int	open_redir_file(t_cmd *cmd, t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIR_IN)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == REDIR_HEREDOC)
	{
		if (cmd->heredoc_file)
			fd = open(cmd->heredoc_file, O_RDONLY);
		else
		{
			ft_putstr_fd("minishell: heredoc file missing\n", STDERR_FILENO);
			fd = -1;
		}
	}
	else if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

/*
** Duplicates fd to stdin or stdout based on redirection type, then closes fd.
** @param fd: The file descriptor to duplicate.
** @param type: The redirection type (determines target: stdin or stdout).
*/
static void	dup_and_close(int fd, t_redir_type type)
{
	int	target_fd;

	if (type == REDIR_IN || type == REDIR_HEREDOC)
		target_fd = STDIN_FILENO;
	else
		target_fd = STDOUT_FILENO;
	if (dup2(fd, target_fd) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		exit(1);
	}
	close(fd);
}

/*
** Processes all redirections for a command.
** Opens each redirection file and redirects stdin/stdout accordingly.
** @param cmd: The command structure containing the redirection list.
*/
void	handle_redir_fds(t_cmd *cmd)
{
	t_redir	*tmp;
	int		fd;

	tmp = cmd->redirs;
	while (tmp)
	{
		fd = open_redir_file(cmd, tmp);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(tmp->filename);
			exit(1);
		}
		dup_and_close(fd, tmp->type);
		tmp = tmp->next;
	}
}

/*
** Connects the child process to the pipeline.
** Redirects stdin from previous pipe and stdout to next pipe if needed.
** @param cmd: The current command structure.
** @param prev_pipe_read: Read fd from previous command (-1 if first).
*/
void	connect_pipes(t_cmd *cmd, int prev_pipe_read)
{
	if (prev_pipe_read != -1)
	{
		if (dup2(prev_pipe_read, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 input");
			close(prev_pipe_read);
			exit(1);
		}
		close(prev_pipe_read);
	}
	if (cmd->next)
	{
		close(cmd->pipefd[0]);
		if (dup2(cmd->pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 output");
			close(cmd->pipefd[1]);
			exit(1);
		}
		close(cmd->pipefd[1]);
	}
}
