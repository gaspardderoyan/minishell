#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int	main(int ac, char **av, char **envp)
{
	char *line; 
	while (1)
	{
		line = readline("minishell> ");
		if (line)
		{
			printf("line: %s\n", line);
			add_history(line);
		}
	}
}
