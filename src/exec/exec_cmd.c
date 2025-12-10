/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/10 18:25:55 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/10 18:26:28 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	run_child_process(t_cmd *cmd, t_data *data)
{
	char	*path; 
	
	path = get_full_path(cmd->args[0], data->env_list);
	if (!path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		ft_free_array(data->env); 
        ft_lstclear(&data->env_list, free);
		exit(127);
	}
	if (execve(path, cmd->args, data->env) == -1)
	{
		perror("minishell");
		free(path);
		exit(126);
	}
}

static void	wait_for_child(pid_t pid, t_data *data)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
	{
		perror("waitpid");
		return ;
	}
	if (WIFEXITED(status))
		data->last_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		data->last_exit_code = 128 + WTERMSIG(status);
}

void	execute_cmd(t_cmd *cmd, t_data *data)
{
	pid_t	pid;

	if (!cmd || !cmd->args || !cmd->args[0])
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
		run_child_process(cmd, data);
	else
	{
		cmd->pid = pid;
		wait_for_child(pid, data);
	}
}
