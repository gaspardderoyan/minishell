/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:01:21 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/18 18:08:58 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_status;

void	signal_handler(int signal)
{
	g_status = signal;
	if (signal == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signal_action(void)
{
	struct sigaction	act;

	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = &signal_handler;
	sigemptyset(&act.sa_mask);
	sigaction(SIGINT, &act, NULL);
	ft_memset(&act, 0, sizeof(act));
	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGQUIT, &act, NULL);
}


void	init_data(t_data *data, char **env)
{
	data->cmd_list = NULL;
	data->tokens = NULL;
	data->env = env;
	data->env_list = init_env_list(env);
	data->last_exit_code = 0;
	data->line = NULL;
	g_status = 0;
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
	if (check_syntax(data->tokens) == FAIL)
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
	set_signal_action();
	while (1)
	{
		g_status = 0;
		data.line = readline("minishell$ ");
		if (g_status == SIGINT)
			data.last_exit_code = 130;
		if (!data.line)
			break ;
		if (data.line[0])
		{
			add_history(data.line);
			if (process_line(&data) == SUCCESS)
				execute_pipeline(&data);
		}
		free_cycle(&data);
	}
	// TODO: add free_permanent() func
	rl_clear_history();
	return (data.last_exit_code);
}
