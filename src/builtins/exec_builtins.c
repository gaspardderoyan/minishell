/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 14:40:37 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/17 17:02:37 by mgregoir         ###   ########.fr       */
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

int	apply_redirections(t_cmd *cmd)
{
	t_redir	*cur;
	int		flags;
	int		ret;

	cur = cmd->redirs;
	while (cur)
	{
		if (cur->type == REDIR_IN)
			ret = setup_redir(cur->filename, O_RDONLY, STDIN_FILENO);
		else
		{
			if (cur->type == REDIR_OUT)
				flags = O_WRONLY | O_CREAT | O_TRUNC;
			else
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
    Fonction principale d'exécution dans le parent
*/
void	execute_builtin_in_parent(t_cmd *cmd, t_data *data)
{
	int	saved_stdin;
	int	saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (apply_redirections(cmd) == -1)
		data->last_exit_code = 1;
	else
		data->last_exit_code = dispatch_builtin(cmd, data);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}
