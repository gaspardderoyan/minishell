/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:01:21 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/12 19:43:27 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>


void	init_data(t_data *data, char **env)
{
	data->cmd_list = NULL;
	data->tokens = NULL;
	data->env = env;
	data->env_list = NULL;
	data->last_exit_code = 0;
	data->line = NULL;
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

int	process_line(t_data *data)
{
	if (lexer(data->line, &data->tokens) == FAIL)
		return (FAIL);
	if (expander(data->tokens, data->env) == FAIL)
		return (FAIL);
	data->cmd_list = parser(data->tokens);
	if (!data->cmd_list)
		return (FAIL);
	return (SUCCESS);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	init_data(&data, env);
	(void)ac;
	(void)av;
	while (1)
	{
		data.line = readline("minishell$ ");
		if (!data.line)
			break ;
		if (data.line[0])
		{
			add_history(data.line);
			process_line(&data);
			print_cmds(data.cmd_list);
		}
		free_cycle(&data);
	}
	// TODO: add free_permanent() func
	clear_history();
	return (data.last_exit_code);
}
