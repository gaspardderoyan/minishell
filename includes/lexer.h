/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:01:09 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/16 16:18:36 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include <stdbool.h>

typedef enum e_state
{
	STATE_IDLE,
	STATE_DQUOTES,
	STATE_QUOTES
}	t_state;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_INPUT,
	TOKEN_OUTPUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

t_token	*token_new(char *value, int type);
t_token	*token_last(t_token *token);
void	token_add_back(t_token **lst, t_token *new);
void	token_add_front(t_token **lst, t_token *new);
int		token_size(t_token *lst);
void	token_clear(t_token **lst);

/* lexer_utils.c */
bool	is_whitespace(char c);
bool	is_operator(char c);
void	skip_whitespace(char *line, int *i);

/* expander_utils.c */
int		get_var_len(char *str);
char	*char_append(char *s, char c);
char	*get_env_value_tab(char *var, char **env);

int	lexer(char *line, t_token **tokens);
int	expander(t_token *tokens, char **env);

/* parser_utils.c */
int		ft_arrlen(char **arr);
char	**ft_append_str(char **arr, char *str);

#endif
