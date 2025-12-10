#include "lexer.h"

t_token	*token_new(char *value, int type)
{
	t_token	*new_node;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->value = value;
	new_node->type = type;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

t_token	*token_last(t_token *token)
{
	if (!token)
		return (NULL);
	while (token->next)
		token = token->next;
	return (token);
}

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

void	token_add_front(t_token **lst, t_token *new)
{
	if (!lst || !new)
		return ;
	new->next = *lst;
	new->prev = NULL;
	if (*lst)
		(*lst)->prev = new;
	*lst = new;
}

int	token_size(t_token *lst)
{
	int	count;

	count = 0;
	while (lst)
	{
		count++;
		lst = lst->next;
	}
	return (count);
}

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
