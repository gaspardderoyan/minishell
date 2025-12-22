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

static char	*generate_heredoc_name(int i)
{
	char	*num;
	char	*name;

	num = ft_itoa(i);
	name = ft_strjoin(".heredoc_tmp_", num);
	free(num);
	return (name);
}

/*
    La boucle de lecture principale.
    Lit stdin ligne par ligne jusqu'à trouver le délimiteur.
*/
static void	fill_heredoc(int fd, char *delimiter)
{
	char	*line;
	int		delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, delimiter_len) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

static int	process_one_heredoc(t_cmd *cmd, t_redir *redir, int unique_id)
{
	int		fd;
	char	*filename;

	filename = generate_heredoc_name(unique_id);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: heredoc");
		free(filename);
		return (-1);
	}
	fill_heredoc(fd, redir->filename);
	close(fd);
	if (cmd->heredoc_file)
	{
		unlink(cmd->heredoc_file);
		free(cmd->heredoc_file);
	}
	cmd->heredoc_file = filename;
	return (0);
}

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
