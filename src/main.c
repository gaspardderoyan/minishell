/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:24:10 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/26 12:56:42 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_status;

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
}

void	remove_empty_tokens(t_token **tokens)
{
	t_token	*curr;
	t_token	*next_node;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr)
	{
		next_node = curr->next;
		if (curr->type == TOKEN_WORD && !ft_strlen_safe(curr->value))
			token_delete(tokens, curr);
		curr = next_node;
	}
}

int	process_line(t_data *data)
{
	if (lexer(data->line, &data->tokens) == FAIL)
		return (FAIL);
	if (expander(data->tokens, data) == FAIL)
		return (FAIL);
	remove_empty_tokens(&data->tokens);
	if (check_syntax(data->tokens, data) == FAIL)
		return (FAIL);
	data->cmd_list = parser(data->tokens);
	if (!data->cmd_list)
		return (FAIL);
	return (SUCCESS);
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	init_data(&data, env, &ac, &av);
	while (1)
	{
		g_status = 0;
		data.line = readline("minishell$ ");
		if (g_status == SIGINT)
			data.last_exit_code = 130;
		if (!data.line)
		{
			if (isatty(STDIN_FILENO))
				ft_putendl_fd("exit", STDERR_FILENO);
			break ;
		}
		if (data.line[0])
		{
			add_history(data.line);
			if (process_line(&data) == SUCCESS)
				execute_pipeline(&data);
		}
		free_cycle(&data);
	}
	free_data(&data);
	rl_clear_history();
	return (data.last_exit_code);
}
