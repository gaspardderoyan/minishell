#include <stdlib.h>

typedef enum	e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_INPUT,
	TOKEN_OUTPUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
} t_token_type ;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
} t_token ;
