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
	t_token *tokens = NULL;
	(void)ac;
	(void)av;
	(void)env;

	while (1)
	{
		line = readline("minishell> ");
		if (line)
		{
			// printf("line: %s\n", line);
			lexer(line, &tokens);
			print_tokens(tokens);
			token_clear(&tokens);
			add_history(line);
		}
	}
}
