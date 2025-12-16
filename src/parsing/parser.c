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
	char	*temp;

	cmds = cmd_new();	
	if (!cmds)
		return (NULL);
	cursor = cmds;
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE)
		{
			cursor->next = cmd_new();
			cursor = cursor->next;
			if (!cursor)
				return (NULL);
		}
		else if (tokens->type == TOKEN_WORD)
		{
			temp = ft_strdup_safe(tokens->value);
			if (!temp)
				return (NULL);
			cursor->args = ft_append_str(cursor->args, temp);
			temp = NULL;
			if (!cursor->args)
				return (NULL);
		}
		else
		{
			if (!tokens->next)
				return (NULL); // TODO: handle syntax error?
			temp = ft_strdup_safe(tokens->next->value);
			if (!temp)
				return (NULL);
			if (tokens->type == TOKEN_APPEND)
				redir = redir_new(REDIR_APPEND, temp);
			else if (tokens->type == TOKEN_INPUT)
				redir = redir_new(REDIR_IN, temp);
			else if (tokens->type == TOKEN_OUTPUT)
				redir = redir_new(REDIR_OUT, temp);
			else if (tokens->type == TOKEN_HEREDOC)
				redir = redir_new(REDIR_HEREDOC, temp);
			temp = NULL;
			if (!redir)
				return (NULL);
			redir_add_back(&cursor->redirs, redir);
			if (tokens->next)
				tokens = tokens->next;
		}
		tokens = tokens->next;
	}
	return (cmds);
}
