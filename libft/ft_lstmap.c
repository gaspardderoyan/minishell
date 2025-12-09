/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 17:45:50 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/20 11:05:55 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Creates a new list by applying a function to each node's content.
 * @param lst  The first node of the list to iterate over.
 * @param f    The function to apply to each node's content.
 * @param del  The function to delete a node's content if needed.
 * @return     A pointer to the new list, or NULL if the allocation fails.
 */
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*newlst;
	t_list	*newnode;
	t_list	*temp;
	void	*temp_content;

	if (!lst || !f || !del)
		return (NULL);
	newlst = NULL;
	temp = lst;
	while (temp)
	{
		temp_content = f(temp->content);
		newnode = ft_lstnew(temp_content);
		if (!newnode)
		{
			del(temp_content);
			ft_lstclear(&newlst, del);
			return (NULL);
		}
		ft_lstadd_back(&newlst, newnode);
		temp = temp->next;
	}
	return (newlst);
}
