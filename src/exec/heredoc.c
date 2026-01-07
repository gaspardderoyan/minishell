/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 14:54:47 by mgregoir          #+#    #+#             */
/*   Updated: 2026/01/07 10:25:14 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Reads lines from stdin until delimiter is found or EOF.
** Writes input to the given file descriptor.
** @param fd: The file descriptor to write to.
** @param delimiter: The string that terminates input.
** @param data: Global data structure for line counts.
** @return: 0 on success, -1 on interruption/error.
*/
static int	fill_heredoc(int fd, char *delimiter, t_data *data)
{
	char	*line;
	int		len;

	len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (print_eof_warning(delimiter, data->heredoc_line) == -1)
				return (-1);
			break ;
		}
		data->line_count++;
		if (ft_strncmp(line, delimiter, len) == 0 && line[len] == '\0')
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
** Opens the temporary heredoc file and initiates filling it.
** Handles opening, error checking, filling, and cleanup on failure.
** @param filename: Path to the temporary file.
** @param delimiter: The heredoc delimiter.
** @param data: Global data structure.
** @return: 0 on success, -1 on error.
*/
static int	open_heredoc_file(char *filename, char *delimiter, t_data *data)
{
	int	fd;
	int	ret;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		return (-1);
	}
	ret = fill_heredoc(fd, delimiter, data);
	close(fd);
	if (ret == -1)
	{
		unlink(filename);
		return (-1);
	}
	return (0);
}

/*
** Coordinates the creation of a single heredoc file.
** Generates filename, opens file, and updates command structure.
** @param cmd: The command associated with the heredoc.
** @param redir: The redirection node containing the delimiter.
** @param unique_id: Counter to ensure unique filenames.
** @param data: Global data structure.
** @return: 0 on success, -1 on error.
*/
static int	process_one_heredoc(t_cmd *cmd, t_redir *redir, int id, t_data *d)
{
	char	*filename;

	filename = generate_heredoc_name(id);
	if (!filename)
		return (-1);
	if (open_heredoc_file(filename, redir->filename, d) == -1)
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
** Main entry point for processing all heredocs in the pipeline.
** Iterates through all commands and their redirections.
** Sets up special signal handling for the duration.
** @param data: Global data structure containing the command list.
** @return: 0 on success, 1 on interruption/error.
*/
int	check_heredoc(t_data *data)
{
	t_cmd	*cmd;
	t_redir	*redir;
	int		i;
	int		bkp;

	bkp = dup(STDIN_FILENO);
	set_signal_heredoc();
	cmd = data->cmd_list;
	i = 0;
	data->heredoc_line = data->line_count;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
				if (process_one_heredoc(cmd, redir, i++, data) < 0)
					return (handle_heredoc_interrupt(data, bkp));
			redir = redir->next;
		}
		cmd = cmd->next;
	}
	close(bkp);
	set_signal_action();
	return (0);
}
