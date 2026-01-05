/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 18:58:33 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/12 18:58:34 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Creates a new redirection node.
** @param type: The redirection type (REDIR_IN, REDIR_OUT, etc.).
** @param filename: The filename for the redirection.
** @return: Pointer to the new redirection, or NULL on allocation failure.
*/
t_redir	*redir_new(t_redir_type type, char *filename)
{
	t_redir	*new_node;

	new_node = malloc(sizeof(t_redir));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->filename = filename;
	new_node->next = NULL;
	return (new_node);
}

/*
** Returns the last redirection in the linked list.
** @param lst: The head of the redirection list.
** @return: The last redirection, or NULL if the list is empty.
*/
static t_redir	*redir_last(t_redir *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/*
** Adds a new redirection at the end of the list.
** @param lst: Pointer to the head of the redirection list.
** @param new: The new redirection to add.
*/
void	redir_add_back(t_redir **lst, t_redir *new)
{
	t_redir	*last_node;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last_node = redir_last(*lst);
	last_node->next = new;
}

/*
** Clears the entire redirection list and frees all allocated memory.
** @param lst: Pointer to the head of the redirection list.
*/
void	redir_clear(t_redir **lst)
{
	t_redir	*temp;

	if (!lst)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->filename)
		{
			free((*lst)->filename);
			(*lst)->filename = NULL;
		}
		free(*lst);
		*lst = temp;
	}
	*lst = NULL;
}
