/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 17:00:47 by gderoyan          #+#    #+#             */
/*   Updated: 2025/01/04 18:00:00 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Creates a new token node.
** @param value: The token string value.
** @param type: The token type (TOKEN_WORD, TOKEN_PIPE, etc.).
** @return: Pointer to the new token, or NULL on allocation failure.
*/
t_token	*token_new(char *value, int type)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = value;
	new_node->type = type;
	new_node->quoted = 0;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

/*
** Returns the last token in the linked list.
** @param token: The head of the token list.
** @return: The last token, or NULL if the list is empty.
*/
t_token	*token_last(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

/*
** Adds a new token at the end of the list.
** Updates prev/next pointers for doubly-linked list.
** @param lst: Pointer to the head of the token list.
** @param new: The new token to add.
*/
void	token_add_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		new->prev = NULL;
		return ;
	}
	last = token_last(*lst);
	last->next = new;
	new->prev = last;
}

/*
** Clears the entire token list and frees all allocated memory.
** @param lst: Pointer to the head of the token list.
*/
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

/*
** Deletes a specific token from the list.
** Updates prev/next pointers and frees the token.
** @param lst: Pointer to the head of the token list.
** @param token: The token to delete.
*/
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
