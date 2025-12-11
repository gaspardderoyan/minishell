/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:01:21 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/11 17:01:21 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "lexer.h"

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
			print_tokens(tokens);
			// TODO: parser
			token_clear(&tokens);
		}
	}
}
