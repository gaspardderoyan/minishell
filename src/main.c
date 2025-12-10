#include "minishell.h"

int	main(void)
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
