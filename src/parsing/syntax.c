/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:52:39 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/23 00:50:24 by gaspardderoya    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Checks if a token type is a redirection operator.
** @param type: The token type to check.
** @return: true if redirection, false otherwise.
*/
static bool	is_redir(t_token_type type)
{
	if (type == TOKEN_INPUT || type == TOKEN_OUTPUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (true);
	return (false);
}

/*
** Removes empty tokens from the token list.
** Called after expansion to clean up empty strings.
** @param tokens: Pointer to the head of the token list.
*/
void	remove_empty_tokens(t_token **tokens)
{
	t_token	*curr;
	t_token	*next_node;

	if (!tokens || !*tokens)
		return ;
	curr = *tokens;
	while (curr)
	{
		next_node = curr->next;
		if (curr->type == TOKEN_WORD && !ft_strlen_safe(curr->value))
			token_delete(tokens, curr);
		curr = next_node;
	}
}

/*
** Validates the syntax of the token list.
** Checks for: pipes at start/end, double pipes, redirections without files.
** @param tokens: The head of the token list.
** @param data: Global data structure (for setting exit code on error).
** @return: SUCCESS (0) if syntax is valid, FAIL (1) otherwise.
*/
int	check_syntax(t_token *tokens, t_data *data)
{
	if (!tokens)
		return (SUCCESS);
	if (tokens->type == TOKEN_PIPE || token_last(tokens)->type == TOKEN_PIPE)
		return (synterr(tokens->next, '|', 0, data), FAIL);
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE && tokens->next
			&& tokens->next->type == TOKEN_PIPE)
			return (synterr(tokens->next, 0, 0, data), FAIL);
		else if (is_redir(tokens->type) && !(tokens->next))
			return (synterr(tokens->next, 0, 1, data), FAIL);
		else if (is_redir(tokens->type) && tokens->next
			&& tokens->next->type != TOKEN_WORD)
			return (synterr(tokens->next, 0, 0, data), FAIL);
		tokens = tokens->next;
	}
	return (SUCCESS);
}
