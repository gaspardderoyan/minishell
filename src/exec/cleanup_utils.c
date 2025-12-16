/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 14:50:13 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/11 14:50:28 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
    Ferme tous les pipes ouverts dans la liste des commandes.
    Utilisé pour nettoyer proprement :
    1. Dans l'enfant : après les dup2, pour ne pas garder de pipes ouverts.
    2. Dans le parent : en cas d'erreur ou à la fin de tout le pipeline.
*/
void	close_all_pipes(t_cmd *cmd_list)
{
	t_cmd	*tmp;

	tmp = cmd_list;
	while (tmp)
	{
		// Fermeture du côté LECTURE
		if (tmp->pipefd[0] != -1)
		{
			close(tmp->pipefd[0]);
			tmp->pipefd[0] = -1; // Sécurité pour éviter double close
		}
		// Fermeture du côté ÉCRITURE
		if (tmp->pipefd[1] != -1)
		{
			close(tmp->pipefd[1]);
			tmp->pipefd[1] = -1;
		}
		tmp = tmp->next;
	}
}

/* 
   Ferme les FDs spécifiques d'une commande (utile pour le nettoyage ponctuel)
   Par exemple : fermer les fichiers ouverts par les redirections (<, >)
   s'ils ont été stockés dans fd_in/fd_out.
*/
void	close_cmd_fds(t_cmd *cmd)
{
	if (cmd->fd_in != -1 && cmd->fd_in != STDIN_FILENO)
	{
		close(cmd->fd_in);
		cmd->fd_in = -1;
	}
	if (cmd->fd_out != -1 && cmd->fd_out != STDOUT_FILENO)
	{
		close(cmd->fd_out);
		cmd->fd_out = -1;
	}
}