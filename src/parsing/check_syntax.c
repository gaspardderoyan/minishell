/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:52:39 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/17 18:34:28 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_syntax(t_token *tokens)
{
	if (tokens->type == TOKEN_PIPE || token_last(tokens)->type == TOKEN_PIPE)
		return (ms_error("unexpected token", NULL), FAIL);
	while (tokens)
	{
		if (tokens->type == TOKEN_PIPE && tokens->next
			&& tokens->next->type == TOKEN_PIPE)
			return (ms_error("double pipe", NULL), FAIL);
		tokens = tokens->next;
	}
	return (SUCCESS);
}
