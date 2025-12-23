/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 14:54:47 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/22 18:07:14 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Generates a unique heredoc temporary filename.
** @param i: Unique identifier number.
** @return: Allocated filename string (e.g., ".heredoc_tmp_0"), or NULL on error.
*/
static char	*generate_heredoc_name(int i)
{
	char	*num;
	char	*name;

	num = ft_itoa(i);
	if (!num)
		return (NULL);
	name = ft_strjoin(".heredoc_tmp_", num);
	free(num);
	return (name);
}

/*
** Reads lines from stdin until delimiter is found or EOF (Ctrl+D).
** Each line is written to the given file descriptor.
** @param fd: File descriptor to write lines to.
** @param delimiter: The heredoc delimiter string.
** @return: 0 on success, -1 if interrupted (Ctrl+D).
*/
static int	fill_heredoc(int fd, char *delimiter)
{
	char	*line;
	int		delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			return (-1);
		if (ft_strncmp(line, delimiter, delimiter_len) == 0
			&& line[delimiter_len] == '\0')
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
	return (0);
}

/*
** Opens a heredoc file and fills it with user input until delimiter.
** @param filename: The temporary file path to create.
** @param delimiter: The heredoc delimiter string.
** @return: 0 on success, -1 on error (file is unlinked on failure).
*/
static int	open_and_fill_heredoc(char *filename, char *delimiter)
{
	int	fd;
	int	ret;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		return (-1);
	}
	ret = fill_heredoc(fd, delimiter);
	close(fd);
	if (ret == -1)
	{
		unlink(filename);
		return (-1);
	}
	return (0);
}

/*
** Processes a single heredoc redirection for a command.
** Creates temporary file, prompts user for input, stores filename in cmd.
** @param cmd: The command structure to attach heredoc file to.
** @param redir: The redirection containing the delimiter.
** @param unique_id: Unique identifier for the heredoc file.
** @return: 0 on success, -1 on error.
*/
static int	process_one_heredoc(t_cmd *cmd, t_redir *redir, int unique_id)
{
	char	*filename;

	filename = generate_heredoc_name(unique_id);
	if (!filename)
		return (-1);
	if (open_and_fill_heredoc(filename, redir->filename) == -1)
	{
		free(filename);
		return (-1);
	}
	if (cmd->heredoc_file)
	{
		unlink(cmd->heredoc_file);
		free(cmd->heredoc_file);
	}
	cmd->heredoc_file = filename;
	return (0);
}

/*
** Processes all heredoc redirections in the command list.
** Called before pipeline execution to create all heredoc temporary files.
** @param data: Global data structure containing cmd_list.
** @return: 0 on success, 1 on error.
*/
int	check_heredoc(t_data *data)
{
	t_cmd	*cmd;
	t_redir	*redir;
	int		i;

	cmd = data->cmd_list;
	i = 0;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				if (process_one_heredoc(cmd, redir, i++) == -1)
					return (1);
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
