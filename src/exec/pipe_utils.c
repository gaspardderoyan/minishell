/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 12:43:41 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/11 13:46:53 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	set_pipe(t_cmd *cmd)
{
	if (!cmd->next)
		return (1);
	// cmd->pipefd[0] : Bout de LECTURE (sera utilisé par le Next)
	// cmd->pipefd[1] : Bout d'ÉCRITURE (sera utilisé par Cmd actuel)
	if (pipe(cmd->pipefd) == -1)
	{
		perror("minishell: pipe");
		return (0);
	}
	return (1);
}

/*
    Ouvre le fichier selon le type de redirection.
    Retourne le FD ouvert ou -1 en cas d'erreur.
*/
static int	open_redir_file(t_redir *redir)
{
	int	fd;

	fd = -1;
	if (redir->type == REDIR_IN)
		fd = open(redir->filename, O_RDONLY);// < input.txt : Lecture seule
	else if (redir->type == REDIR_HEREDOC)
		fd = open(redir->filename, O_RDONLY);// << DELIM : On lit depuis le fichier du heredoc
	else if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);// > output.txt : Écriture, Création, Troncature (écrase)
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);// >> output.txt : Écriture, Création, Ajout à la fin
	return (fd);
}

static void	dup_and_close(int fd, t_redir_type type)
{
	int	target_fd;

	if (type == REDIR_IN || type == REDIR_HEREDOC)
		target_fd = STDIN_FILENO;
	else
		target_fd = STDOUT_FILENO;
	if (dup2(fd, target_fd) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		exit(1);
	}
	close(fd);
}

void	handle_redir_fds(t_cmd *cmd)
{
	t_redir	*tmp;
	int		fd;

	tmp = cmd->redirs;
	while (tmp)
	{
		fd = open_redir_file(tmp);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(tmp->filename);
			exit(1);
		}
		dup_and_close(fd, tmp->type);
		tmp = tmp->next;
	}
}

/*
    Connecte les entrées/sorties standards aux pipes.
    - cmd : la commande actuelle.
    - prev_pipe_read : le bout de lecture du pipe de la commande PRÉCÉDENTE.
      (Doit être -1 si c'est la première commande).
*/
void	connect_pipes(t_cmd *cmd, int prev_pipe_read)
{
	// 1. GESTION ENTRÉE (Connexion au pipe précédent)
	// Si on n'est pas la première commande, on lit depuis le pipe précédent
	if (prev_pipe_read != -1)
	{
		if (dup2(prev_pipe_read, STDIN_FILENO) == -1)
		{
			perror("minishell: dup2 input");
			close(prev_pipe_read);
			exit(1);
		}
		close(prev_pipe_read);
	}
	if (cmd->next)
	{
		// dans l'enfant, on ferme le bout de LECTURE du pipe actuel car on ne va faire qu'écrire dedans.
		close(cmd->pipefd[0]);
		if (dup2(cmd->pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("minishell: dup2 output");
			close(cmd->pipefd[1]);
			exit(1);
		}
		// Une fois connecté à STDOUT, on ferme l'original
		close(cmd->pipefd[1]);
	}
}
