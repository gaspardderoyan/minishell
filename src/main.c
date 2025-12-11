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

int	main(int ac, char **av, char **env)
{
	char *line; 
	t_data	data;
	t_token *tokens = NULL;
	t_cmd	*cmds = NULL;
	(void)ac;
	(void)av;
	data.env = env;

	while (1)
	{
		line = readline("minishell> ");
		if (line)
		{
			add_history(line);
			lexer(line, &tokens);
			expander(tokens, data.env);
			// print_tokens(tokens);
			cmds = parser(tokens);	
			print_cmds(cmds);
			token_clear(&tokens);
		}
	}
}
