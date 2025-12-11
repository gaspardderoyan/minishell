#include "minishell.h"
#include "lexer.h"

/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* utils_array.c                                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: student <student@42.fr>                    +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/12/11 17:35:00 by student           #+#    #+#             */
/* Updated: 2025/12/11 17:35:00 by student          ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

int	ft_arrlen(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

char	**ft_append_str(char **arr, char *str)
{
	char	**new_arr;
	int		len;
	int		i;

	if (!str)
		return (arr);
	len = ft_arrlen(arr);
	new_arr = malloc(sizeof(char *) * (len + 2));
	if (!new_arr)
		return (NULL);
	i = -1;
	while (++i < len)
		new_arr[i] = arr[i];
	new_arr[i] = str;
	new_arr[i + 1] = NULL;
	if (arr)
		free(arr);
	return (new_arr);
}

t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmds;
	t_cmd	*cursor;
	t_redir	*redir;

	cmds = cmd_new();	
	if (!cmds)
		exit(0);
	cursor = cmds;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			cursor = cursor->next;
			cursor = cmd_new();
		}
		else if (tokens->type == TOKEN_WORD)
			cursor->args = ft_append_str(cursor->args, tokens->value);
		else
		{
			if (tokens->type == TOKEN_APPEND)
				redir = redir_new(REDIR_APPEND, tokens->value);
			else if (tokens->type == TOKEN_INPUT)
				redir = redir_new(REDIR_IN, tokens->value);
			else if (tokens->type == TOKEN_OUTPUT)
				redir = redir_new(REDIR_OUT, tokens->value);
			else if (tokens->type == TOKEN_HEREDOC)
				redir = redir_new(REDIR_HEREDOC, tokens->value);
			if (!redir)
				exit(0);
			redir_add_back(&cursor->redirs, redir);
		}
		tokens = tokens->next;
	}
	return (cmds);
}
