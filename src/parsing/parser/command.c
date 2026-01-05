/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gaspardderoyan <marvin@42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:56:05 by gaspardderoya     #+#    #+#             */
/*   Updated: 2025/12/16 16:56:11 by gaspardderoya    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Initializes a new command node.
** Sets file descriptors to standard input/output (0/1) by default.
** Sets PID to -1 to indicate no process started yet.
*/
t_cmd	*cmd_new(void)
{
	t_cmd	*new_node;

	new_node = malloc(sizeof(t_cmd));
	if (!new_node)
		return (NULL);
	new_node->args = NULL;
	new_node->redirs = NULL;
	new_node->next = NULL;
	new_node->cmd_path = NULL;
	new_node->heredoc_file = NULL;
	new_node->pid = -1;
	new_node->fd_in = 0;
	new_node->fd_out = 1;
	new_node->pipefd[0] = -1;
	new_node->pipefd[1] = -1;
	return (new_node);
}

/*
** Returns the last command node in the linked list.
** @param lst: The head of the command list.
** @return: The last command node, or NULL if the list is empty.
*/
static t_cmd	*cmd_last(t_cmd *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

/*
** Adds a new command node at the end of the list.
** @param lst: Pointer to the head of the command list.
** @param new: The new command node to add.
*/
void	cmd_add_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*last_node;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	last_node = cmd_last(*lst);
	last_node->next = new;
}

/*
** Helper to free the char **args array.
** If you have this in your libft, you can remove this static function
** and call your libft version in cmd_clear.
*/
static void	free_args(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return ;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

/*
** Clears the command list and all nested structures.
** Frees args, redirections, paths, and heredoc files.
** @param lst: Pointer to the head of the command list.
*/
void	cmd_clear(t_cmd **lst)
{
	t_cmd	*temp;

	if (!lst)
		return ;
	while (*lst)
	{
		temp = (*lst)->next;
		if ((*lst)->args)
			free_args((*lst)->args);
		if ((*lst)->redirs)
			redir_clear(&(*lst)->redirs);
		if ((*lst)->cmd_path)
			free((*lst)->cmd_path);
		if ((*lst)->heredoc_file)
			free((*lst)->heredoc_file);
		free(*lst);
		*lst = temp;
	}
	*lst = NULL;
}
