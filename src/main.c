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


void	init_data(t_data *data, char **env)
{
	data->cmd_list = NULL;
	data->tokens = NULL;
	data->env = env;
	data->env_list = NULL;
	data->last_exit_code = 0; // TODO: is this right? ie. default 0
	data->line = NULL;
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

			// TODO: extract to process_line() func
			lexer(data.line, &data.tokens);
			expander(data.tokens, data.env);
			data.cmd_list = parser(data.tokens);	
			print_cmds(data.cmd_list);
		}
		// TODO: add free_cycle() func
		token_clear(&data.tokens);
	}
	// TODO: add free_permanent() func
	return (data.last_exit_code);
}
