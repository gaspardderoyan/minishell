/* ************************************************************************** */
/* */
/* :::      ::::::::   */
/* redir_utils.c                                      :+:      :+:    :+:   */
/* +:+ +:+         +:+     */
/* By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/* +#+#+#+#+#+   +#+           */
/* Created: 2025/12/11 17:10:00 by gderoyan          #+#    #+#             */
/* Updated: 2025/12/11 17:10:00 by gderoyan         ###   ########.fr       */
/* */
/* ************************************************************************** */

#include "minishell.h"

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

t_redir	*redir_last(t_redir *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

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