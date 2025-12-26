/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:52:39 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/23 00:50:24 by gaspardderoya    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	is_redir(t_token_type type)
{
	if (type == TOKEN_INPUT || type == TOKEN_OUTPUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (true);
	return (false);
}

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
