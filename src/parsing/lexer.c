/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:42:37 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/16 15:42:38 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Determines the length of an operator token (1 or 2 characters).
** @param str: The string starting with an operator.
** @return: 2 if double operator (<<, >>), 1 otherwise.
*/
static int	get_operator_len(char *str)
{
	if ((*str == '>' || *str == '<') && *str == *(str + 1))
		return (2);
	return (1);
}

/*
** Skips over quoted content until the closing quote is found.
** @param line: The input line.
** @param i: Pointer to the current index (updated to after closing quote).
*/
static void	skip_quote_content(char *line, int *i)
{
	char	quote_char;

	quote_char = line[*i];
	(*i)++;
	while (line[*i] && line[*i] != quote_char)
		(*i)++;
	if (line[*i] == quote_char)
		(*i)++;
}

/*
** Calculates the length of a word token, handling quoted sections.
** @param line: The input line.
** @param start: The starting index of the word.
** @return: The length of the word token.
*/
static int	get_word_len(char *line, int start)
{
	int	i;

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

/*
** Identifies the token type based on its string value.
** @param str: The token string.
** @return: The corresponding token type (TOKEN_PIPE, TOKEN_INPUT, etc.).
*/
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

/*
** Tokenizes an input line into a linked list of tokens.
** @param line: The input line to tokenize.
** @param tokens: Pointer to the token list head (will be populated).
** @return: SUCCESS (0) on success, FAIL (1) on malloc failure.
*/
int	lexer(char *line, t_token **tokens)
{
	int		i;
	int		len;
	char	*token_str;
	t_token	*token;

	i = 0;
	while (line[i])
	{
		skip_whitespace(line, &i);
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
