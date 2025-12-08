#include <stdio.h>
#include <readline/readline.h>

int	main(int ac, char **av, char **envp)
{
	char *line; 
	while (*envp)
	{
		printf("envp: %s\n", *envp);
		envp++;
	}
	while (1)
	{
		line = readline("minishell> ");
		if (line)
			printf("line: %s\n", line);
	}
}
