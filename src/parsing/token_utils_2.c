/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:13:14 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/24 18:25:04 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	token_clear(t_token **lst)
{
	t_token	*tmp;

	if (!lst)
		return ;
	while (*lst)
	{
		tmp = (*lst)->next;
		if ((*lst)->value)
		{
			free((*lst)->value);
			(*lst)->value = NULL;
		}
		free(*lst);
		*lst = tmp;
	}
	*lst = NULL;
}

void	token_delete(t_token **lst, t_token *token)
{
	if (!token)
		return ;
	if (token->prev)
		token->prev->next = token->next;
	if (token->next)
		token->next->prev = token->prev;
	if (token == *lst)
		*lst = token->next;
	if (token->value)
		free(token->value);
	free(token);
}
