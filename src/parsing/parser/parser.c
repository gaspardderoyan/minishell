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

#include "minishell.h"

/*
** Handles a pipe token by creating a new command node and linking it.
** Moves the cursor to the new command node.
** @param cursor: Pointer to the current command node pointer.
** @return: The new command node, or NULL on allocation failure.
*/
static t_cmd	*handle_pipe(t_cmd **cursor)
{
	(*cursor)->next = cmd_new();
	*cursor = (*cursor)->next;
	if (!*cursor)
		return (NULL);
	return (*cursor);
}

/*
** Handles a word token by adding its value to the current command's arguments.
** Duplicates the token value before adding.
** @param cursor: The current command node.
** @param token: The word token to handle.
** @return: 1 on success, 0 on allocation failure.
*/
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

/*
** Handles a redirection token by creating a new redirection node.
** Adds the redirection to the current command's redirection list.
** @param cursor: The current command node.
** @param token: The redirection operator token.
** @return: The token after the redirection (filename), or NULL on error.
*/
static t_token	*handle_redir(t_cmd *cursor, t_token *token)
{
	t_redir	*redir;
	char	*temp;

	if (!token->next)
		return (NULL);
	temp = ft_strdup_safe(token->next->value);
	if (!temp)
		return (NULL);
	redir = create_redir(token, temp, token->next->quoted);
	if (!redir)
		return (NULL);
	redir_add_back(&cursor->redirs, redir);
	return (token->next);
}

/*
** Processes a single token, dispatching it to the appropriate handler
** (pipe, word, or redirection).
** @param cursor: Pointer to the current command node pointer.
** @param tokens: Pointer to the current token pointer.
** @return: 1 on success, 0 on failure.
*/
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

/*
** Main parser function. Converts a linked list of tokens into a
** command pipeline.
** Creates a new command list and populates it by processing each token.
** @param tokens: The head of the token linked list.
** @return: The head of the command linked list, or NULL on error.
*/
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
