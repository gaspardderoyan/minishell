/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:00:00 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/16 17:00:00 by gderoyan         ###   ########.fr       */
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

t_redir	*create_redir(t_token *token, char *filename)
{
	if (token->type == TOKEN_APPEND)
		return (redir_new(REDIR_APPEND, filename));
	else if (token->type == TOKEN_INPUT)
		return (redir_new(REDIR_IN, filename));
	else if (token->type == TOKEN_OUTPUT)
		return (redir_new(REDIR_OUT, filename));
	else if (token->type == TOKEN_HEREDOC)
		return (redir_new(REDIR_HEREDOC, filename));
	return (NULL);
}
