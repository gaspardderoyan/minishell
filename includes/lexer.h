/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:01:09 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/11 17:43:13 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef enum	e_state
{
	STATE_IDLE,
	STATE_DQUOTES,
	STATE_QUOTES
}	t_state;

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

int	lexer(char *line, t_token **tokens);
void	expander(t_token *tokens, char **env);
