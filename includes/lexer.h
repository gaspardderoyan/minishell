#include "minishell.h"

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
	struct s_token	*prev;
} t_token ;

t_token	*token_new(char *value, int type);
t_token	*token_last(t_token *token);
void	token_add_back(t_token **lst, t_token *new);
void	token_add_front(t_token **lst, t_token *new);
int		token_size(t_token *lst);
void	token_clear(t_token **lst);
