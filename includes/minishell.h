/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:58:43 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/18 18:03:34 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "lexer.h"
# include "libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
# include <signal.h>
# define SUCCESS 0
# define FAIL 1

/* -------------------------------------------------------
   1. REDIRECTION TYPES
   Used to identify what kind of redirection a node is.
------------------------------------------------------- */
typedef enum e_redir_type
{
	REDIR_IN,     // <   (Input from file)
	REDIR_OUT,    // >   (Output to file, overwrite)
	REDIR_APPEND, // >>  (Output to file, append)
	REDIR_HEREDOC // <<  (Input from terminal until delimiter)
}			t_redir_type;

/* -------------------------------------------------------
   2. REDIRECTION LIST
   Stores a list of files/delimiters associated with a command.
------------------------------------------------------- */
typedef struct s_redir
{
	t_redir_type type;    // The type (from the enum above)
	char *filename;       // The filename (or delimiter for heredoc)
	struct s_redir *next; // Pointer to the next redirection node
}			t_redir;

/* -------------------------------------------------------
   3. COMMAND TABLE
   One node per command. A pipeline is a linked list of these.
------------------------------------------------------- */
typedef struct s_cmd
{
	// --- PARSER FIELDS (Filled by You) ---
	char **args;        // The command and its flags
	t_redir *redirs;    // Linked list of redirections
	struct s_cmd *next; // Pointer to the next command (pipe)
						// --- EXECUTOR FIELDS (Filled by Teammate) ---
	char *cmd_path;     // /usr/bin/ls (ou NULL si builtin/introuvable)
	pid_t pid;          // PID du processus fils (-1 si builtin)
	int fd_in;          // FD final d'entrée (0, fichier, ou pipe_read)
	int fd_out;         // FD final de sortie (1, fichier, ou pipe_write)
	int pipefd[2];      // Le pipe vers la commande SUIVANTE
						// pipe_fd[0] sera lu par cmd->next
						// pipe_fd[1] sera écrit par cmd actuel
	char *heredoc_file; // (Optionnel) Nom du fichier temporaire si <<
						// Utile pour le unlink() lors du cleanup
}			t_cmd;

typedef struct s_data
{
	t_cmd	*cmd_list;      // Le début de ta liste chaînée de commandes
	char	**env;    		// L'environnement converti pour execve
	t_list	*env_list;      // L'environnement manipulable (export/unset)
    int		last_exit_code; // Le code de retour de la dernière commande ($?)
	t_token *tokens;    // The tokens lists
	char	*line;
} t_data;

/******    child.c   ******/
void	exec_child(t_cmd *cmd, t_data *data, int prev_read_fd);

/******    cleanup_utils.c    ******/
void	close_all_pipes(t_cmd *cmd_list);
void	close_cmd_fds(t_cmd *cmd);

/******    env.c    ******/
t_list	*init_env_list(char **env);
char	**env_list_to_array(t_list *env_list);

/******    path.c   ******/
char	*get_full_path(char *cmd, t_list *env_list);
void	init_cmd_path(t_cmd *cmd, t_data *data);

/******    pipe_utils.c    ******/
int		set_pipe(t_cmd *cmd);
void	handle_redir_fds(t_cmd *cmd);
void	connect_pipes(t_cmd *cmd, int prev_pipe_read);

/******    pipeline.c    ******/
void	execute_pipeline(t_data *data);

t_cmd		*cmd_new(void);
t_cmd		*cmd_last(t_cmd *lst);
void		cmd_add_back(t_cmd **lst, t_cmd *new);
void		cmd_clear(t_cmd **lst);
void		free_cycle(t_data *data);

t_redir		*redir_new(t_redir_type type, char *filename);
t_redir		*redir_last(t_redir *lst);
void		redir_add_back(t_redir **lst, t_redir *new);
void		redir_clear(t_redir **lst);
t_redir		*create_redir(t_token *token, char *filename);

/*
 * Utils to delete
 * */
void		print_cmds(t_cmd *cmds);
void		print_tokens(t_token *tokens);

t_cmd		*parser(t_token *tokens);

#endif
