#include "../../includes/minishell.h"

static void	error_exit(t_cmd *cmd, t_data *data, char *msg, int err_code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd->args && cmd->args[0])
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_free_array(data->env);
	ft_lstclear(&data->env_list, free);
	exit(err_code);
}


static void	do_execve(t_cmd *cmd, t_data *data)
{
	char    **env_array;

	if (!cmd->args || !cmd->args[0])
	{
		//ft_free_array(data->env);
		ft_lstclear(&data->env_list, free);
		exit(0);
	}
	init_cmd_path(cmd, data);
	if (!cmd->cmd_path)
		error_exit(cmd, data, ": command not found\n", 127);
	if (execve(cmd->cmd_path, cmd->args, env_array) == -1)
	{
		perror("minishell");
		ft_free_array(env_array);
		ft_lstclear(&data->env_list, free);
		exit(126);
	}
}

void	exec_child(t_cmd *cmd, t_data *data, int prev_read_fd)
{
	connect_pipes(cmd, prev_read_fd);
	handle_redir_fds(cmd);
	close_all_pipes(data->cmd_list);
	do_execve(cmd, data);
}
