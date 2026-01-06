/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 14:49:11 by gderoyan          #+#    #+#             */
/*   Updated: 2026/01/06 14:49:12 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_data(t_data *data, char **env, int *ac, char ***av)
{
	(void)ac;
	(void)av;
	data->cmd_list = NULL;
	data->tokens = NULL;
	data->env = copy_env(env);
	data->env_list = init_env_list(env);
	data->last_exit_code = 0;
	data->line = NULL;
	data->stdin_backup = -1;
	data->stdout_backup = -1;
	g_status = 0;
	set_signal_action();
}

void	free_cycle(t_data *data)
{
	if (data->tokens)
		token_clear(&data->tokens);
	if (data->cmd_list)
		cmd_clear(&data->cmd_list);
	if (data->line)
		free(data->line);
	data->tokens = NULL;
	data->cmd_list = NULL;
	data->line = NULL;
}

void	free_data(t_data *data)
{
	if (data->env)
		ft_free_array(data->env);
	if (data->env_list)
		ft_lstclear(&data->env_list, free);
	data->env = NULL;
	data->env_list = NULL;
	rl_clear_history();
}
