#include "minishell.h"

bool	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (true);
	return (false);
}

bool	is_operator(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (true);
	return (false);
}

int	get_operator_len(char *str)
{
	if ((*str == '>' || *str == '<') && *str == *(str + 1))
		return (2);
	return (1);
}

void    skip_quote_content(char *line, int *i)
{
    char    quote_char;

    quote_char = line[*i];
    (*i)++;
    while (line[*i] && line[*i] != quote_char)
        (*i)++;
    if (line[*i] == quote_char)
        (*i)++;
}

int     get_word_len(char *line, int start)
{
    int i;

    i = start;
    while (line[i] && !is_whitespace(line[i]) && !is_operator(line[i]))
    {
        if (line[i] == '\'' || line[i] == '"')
            skip_quote_content(line, &i);
        else
            i++;
    }
    return (i - start);
}

t_token_type	id_token(char *str)
{
	if (*str == '|')
		return (TOKEN_PIPE);
	else if (*str == '>' && *(str + 1) == '>')
		return (TOKEN_APPEND);
	else if (*str == '>')
		return (TOKEN_OUTPUT);
	else if (*str == '<' && *(str + 1) == '<')
		return (TOKEN_HEREDOC);
	else if (*str == '<')
		return (TOKEN_INPUT);
	return (TOKEN_WORD);
}

int	lexer(char *line, t_token **tokens)
{
	int		i;
	int		len;
	char	*token_str;
	t_token	*token;

	i = 0;
	while (line[i])
	{
		while (is_whitespace(line[i]) && line[i])
			i++;
		if (!line[i])
			break ;
		if (is_operator(line[i]))
			len = get_operator_len(&line[i]);
		else
			len = get_word_len(line, i);
		token_str = ft_substr(line, i, len);
		if (!token_str)
			return (FAIL);
		token = token_new(token_str, id_token(token_str));
		if (!token)
			return (free(token_str), FAIL);
		token_add_back(tokens, token);
		i += len;
	}
	return (SUCCESS);
}
