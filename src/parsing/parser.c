/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:58:23 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/12 18:58:24 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

static t_cmd	*handle_pipe(t_cmd **cursor)
{
	(*cursor)->next = cmd_new();
	*cursor = (*cursor)->next;
	if (!*cursor)
		return (NULL);
	return (*cursor);
}

static int	handle_word(t_cmd *cursor, t_token *token)
{
	char	*temp;

	temp = ft_strdup_safe(token->value);
	if (!temp)
		return (0);
	cursor->args = ft_append_str(cursor->args, temp);
	if (!cursor->args)
		return (0);
	return (1);
}

static t_token	*handle_redir(t_cmd *cursor, t_token *token)
{
	t_redir	*redir;
	char	*temp;

	if (!token->next)
		return (NULL);
	temp = ft_strdup_safe(token->next->value);
	if (!temp)
		return (NULL);
	redir = create_redir(token, temp);
	if (!redir)
		return (NULL);
	redir_add_back(&cursor->redirs, redir);
	return (token->next);
}

static int	process_token(t_cmd **cursor, t_token **tokens)
{
	if ((*tokens)->type == TOKEN_PIPE)
	{
		if (!handle_pipe(cursor))
			return (0);
	}
	else if ((*tokens)->type == TOKEN_WORD)
	{
		if (!handle_word(*cursor, *tokens))
			return (0);
	}
	else
	{
		*tokens = handle_redir(*cursor, *tokens);
		if (!*tokens)
			return (0);
	}
	return (1);
}

t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*cursor;

	cmds = cmd_new();
	if (!cmds)
		return (NULL);
	cursor = cmds;
	while (tokens)
	{
		if (!process_token(&cursor, &tokens))
			return (NULL);
		tokens = tokens->next;
	}
	return (cmds);
}
