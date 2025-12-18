/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:52:39 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/17 19:13:08 by gderoyan         ###   ########.fr       */
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

int	check_syntax(t_token *tokens)
{
	if (tokens->type == TOKEN_PIPE || token_last(tokens)->type == TOKEN_PIPE)
		return (ms_error("unexpected token", NULL), FAIL);
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE && tokens->next
			&& tokens->next->type == TOKEN_PIPE)
			return (ms_error("double pipe", NULL), FAIL);
		else if (is_redir(tokens->type) && !(tokens->next))
			return (ms_error("invalid redirection", NULL), FAIL);
		else if (is_redir(tokens->type) && tokens->next
			&& tokens->next->type != TOKEN_WORD)
			return (ms_error("invalid redirection", NULL), FAIL);
		tokens = tokens->next;
	}
	return (SUCCESS);
}
