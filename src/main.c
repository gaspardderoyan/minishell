/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 19:24:10 by gderoyan          #+#    #+#             */
/*   Updated: 2026/01/06 14:49:15 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_status = 0;

static int	process_line(t_data *data)
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

static char	*read_input(void)
{
	char	*line;
	size_t	len;

	if (isatty(STDIN_FILENO))
		return (readline("minishell$ "));
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (line);
}

static void	shell_loop(t_data *data)
{
	while (1)
	{
		g_status = 0;
		data->line = read_input();
		if (g_status == SIGINT)
			data->last_exit_code = 130;
		if (!data->line)
		{
			if (isatty(STDIN_FILENO))
				ft_putendl_fd("exit", STDERR_FILENO);
			break ;
		}
		if (data->line[0])
		{
			if (isatty(STDIN_FILENO))
				add_history(data->line);
			if (process_line(data) == SUCCESS)
				execute_pipeline(data);
		}
		free_cycle(data);
	}
}

int	main(int ac, char **av, char **env)
{
	t_data	data;

	init_data(&data, env, &ac, &av);
	shell_loop(&data);
	free_cycle(&data);
	free_data(&data);
	return (data.last_exit_code);
}
