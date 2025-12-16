#include "../../includes/minishell.h"

/*
    Helper pour gérer l'erreur "Command not found" ou "Exec format error"
    et nettoyer la mémoire avant de quitter.
*/
static void	error_exit(t_cmd *cmd, t_data *data, char *msg, int err_code)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd->args && cmd->args[0])
		ft_putstr_fd(cmd->args[0], 2);
	ft_putstr_fd(msg, 2);
	ft_free_array(data->env);
	ft_lstclear(&data->env_list, free);
	exit(err_code);
}

/*
    Partie 2 de l'enfant : Trouve le path et lance execve.
    Isolée pour réduire la taille de exec_child.
*/
static void	do_execve(t_cmd *cmd, t_data *data)
{
	// Sécurité crash si commande vide (ex: < infile)
	if (!cmd->args || !cmd->args[0])
	{
		ft_free_array(data->env);
		ft_lstclear(&data->env_list, free);
		exit(0);
	}

	init_cmd_path(cmd, data);

	if (!cmd->cmd_path)
		error_exit(cmd, data, ": command not found\n", 127);

	if (execve(cmd->cmd_path, cmd->args, data->env) == -1)
	{
		perror("minishell"); // Affiche l'erreur système exacte (ex: Is a directory)
		ft_free_array(data->env);
		ft_lstclear(&data->env_list, free);
		exit(126);
	}
}

/*
    Fonction principale de l'enfant (appelée par execute_pipeline)
*/
void	exec_child(t_cmd *cmd, t_data *data, int prev_read_fd)
{
	// 1. Mise en place des tuyaux
	connect_pipes(cmd, prev_read_fd);

	// 2. Mise en place des redirections (< >)
	handle_redir_fds(cmd);

	// 3. Fermeture des copies inutiles de FDs
	close_all_pipes(data->cmd_list);

	// 4. Exécution finale
	do_execve(cmd, data);
}
