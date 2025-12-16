/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 14:56:49 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/11 17:47:06 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/* 
   PARTIE 1 : Ce que fait l'ENFANT
   C'est ici qu'on branche tout et qu'on lance la commande.
*/
static void	exec_child(t_cmd *cmd, t_data *data, int prev_read_fd)
{
	// 1. Branchement des tuyaux (Pipe précédent -> Entrée, Pipe actuel -> Sortie)
	connect_pipes(cmd, prev_read_fd);
	// 2. Branchement des redirections (<, >, <<, >>)
	// Elles écrasent les pipes si nécessaire.
	handle_redir_fds(cmd);
	// 3. Nettoyage : L'enfant a tout dupliqué, il doit fermer les originaux
	// pour ne pas laisser traîner de FDs ouverts.
	close_all_pipes(data->cmd_list);
    // Note : prev_read_fd est fermé dans connect_pipes, pas besoin de le refaire
	// 4. Recherche et Exécution
	// (On réutilise ta logique de la Phase 2 ici)
	init_cmd_path(cmd, data); // Trouve le chemin (/bin/ls)
	if (!cmd->cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		ft_free_array(data->env);
		ft_lstclear(&data->env_list, free);
		exit(127);
	}
	if (execve(cmd->cmd_path, cmd->args, data->env) == -1)
	{
		perror("minishell");
		ft_free_array(data->env);
		ft_lstclear(&data->env_list, free);
		exit(126);
	}
}

/* 
   PARTIE 2 : Attendre tous les enfants
   On récupère le code de retour de la DERNIÈRE commande uniquement ($?).
*/
static void	wait_all_children(t_data *data)
{
	t_cmd	*cmd;
	int		status;

	cmd = data->cmd_list;
	while (cmd)
	{
		// On attend le processus associé à cette commande
		if (cmd->pid != -1)
		{
			waitpid(cmd->pid, &status, 0);
			// Si c'est la dernière commande, on stocke le code de retour
			if (cmd->next == NULL)
			{
				if (WIFEXITED(status))
					data->last_exit_code = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
					data->last_exit_code = 128 + WTERMSIG(status);
			}
		}
		cmd = cmd->next;
	}
}

/* 
   PARTIE 3 : L'Orchestrateur (Main Loop)
*/
void	execute_pipeline(t_data *data)
{
	t_cmd	*cmd;
	int		prev_read_fd; // Le bout de lecture du pipe précédent

	cmd = data->cmd_list;
	prev_read_fd = -1; // -1 car pas de pipe avant la 1ère commande

	while (cmd)
	{
		// 1. Préparer le pipe vers le SUIVANT (si next existe)
		if (!set_pipe(cmd))
			return ; // Erreur pipe
		// 2. Fork
		cmd->pid = fork();
		if (cmd->pid == -1)
		{
			perror("fork");
			// Gérer le nettoyage et exit loop
			break ;
		}
		// 3. Aiguillage
		if (cmd->pid == 0)
		{
			// --> ENFANT : Exécute et meurt
			exec_child(cmd, data, prev_read_fd);
		}
		else
		{
			// --> PARENT : Prépare le terrain pour la suite
			// A. Fermer le bout d'écriture du pipe qu'on vient de créer
			//    C'est vital : sinon le pipe reste ouvert et l'enfant suivant
			//    ne recevra jamais EOF.
			if (cmd->next)
				close(cmd->pipefd[1]);
			// B. Fermer le bout de lecture du pipe PRÉCÉDENT
			//    (Celui qu'on a passé à l'enfant actuel, on n'en a plus besoin)
			if (prev_read_fd != -1)
				close(prev_read_fd);
			// C. Sauvegarder le bout de lecture du pipe ACTUEL
			//    Il deviendra le "prev_read_fd" pour la prochaine itération.
			if (cmd->next)
				prev_read_fd = cmd->pipefd[0];
		}
		cmd = cmd->next;
	}
	// 4. Une fois tout lancé, on attend tout le monde
	wait_all_children(data);
}
