/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:01:21 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/11 17:56:57 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

static char *get_redir_type(t_redir_type type)
{
    if (type == REDIR_IN)
        return ("< (IN)");
    if (type == REDIR_OUT)
        return ("> (OUT)");
    if (type == REDIR_APPEND)
        return (">> (APPEND)");
    if (type == REDIR_HEREDOC)
        return ("<< (HEREDOC)");
    return ("UNKNOWN");
}

static void print_args(char **args)
{
    int i;

    i = 0;
    if (!args)
        return ;
    while (args[i])
    {
        printf("[%s] ", args[i]);
        i++;
    }
}

static void print_redirs(t_redir *redirs)
{
    while (redirs)
    {
        printf("\n\t  %s : %s", get_redir_type(redirs->type), redirs->filename);
        redirs = redirs->next;
    }
}

void print_cmds(t_cmd *cmds)
{
    int i;

    i = 0;
    printf("\n--- COMMAND LIST ---\n");
    while (cmds)
    {
        printf("CMD %d:\n", i);
        
        printf("\tArgs: ");
        if (cmds->args)
            print_args(cmds->args);
        else
            printf("(empty)");

        printf("\n\tRedirs: ");
        if (cmds->redirs)
            print_redirs(cmds->redirs);
        else
            printf("(none)");
        
        printf("\n\tPipe: ");
        if (cmds->next)
            printf("Yes (-> Next CMD)\n");
        else
            printf("No (End of Line)\n");
        
        printf("--------------------\n");
        cmds = cmds->next;
        i++;
    }
    printf("\n");
}

char	*get_token_name(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("TOKEN_WORD");
	if (type == TOKEN_PIPE)
		return ("TOKEN_PIPE");
	if (type == TOKEN_INPUT)
		return ("TOKEN_INPUT");
	if (type == TOKEN_OUTPUT)
		return ("TOKEN_OUTPUT");
	if (type == TOKEN_HEREDOC)
		return ("TOKEN_HEREDOC");
	if (type == TOKEN_APPEND)
		return ("TOKEN_APPEND");
	return ("UNKNOWN");
}

void	print_tokens(t_token *tokens)
{
	int	i = 0;
	while (tokens)
	{
		printf("%d: %s (%s)\n", i, tokens->value, get_token_name(tokens->type));
		i++;
		tokens=tokens->next;
	}
}

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
