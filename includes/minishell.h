/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:58:43 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/24 15:51:40 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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
# include <limits.h>
# include <signal.h>
# include <sys/stat.h>
# include <errno.h>
# define SUCCESS 0
# define FAIL 1

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef enum e_state
{
	STATE_IDLE,
	STATE_DQUOTES,
	STATE_QUOTES
}	t_state;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_INPUT,
	TOKEN_OUTPUT,
	TOKEN_HEREDOC,
	TOKEN_APPEND,
}	t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

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
	int		stdin_backup;
	int		stdout_backup;
} t_data;

/******    builtin_cd.c    ******/
int		builtin_cd(char **args, t_data *data);

/******   builtin_exit.c    ******/
int		builtin_exit(char **args, t_data *data);

/******    builtin_export.c    ******/
int		builtin_export(char **args, t_data *data);

/******    builtin_rest.c   ******/
int		builtin_echo(char **args);
int		builtin_pwd(void);
int		builtin_env(t_list *env);
int		builtin_unset(char **args, t_data *data);

/******    builtins_dispatch.c   ******/
int		is_builtin(char *cmd);
int		is_modifier_builtin(char *cmd);
int		dispatch_builtin(t_cmd *cmd, t_data *data);

/******    builtins_exec.c    ******/
int		apply_redirections(t_cmd *cmd);
void	execute_builtin_in_parent(t_cmd *cmd, t_data *data);

/******    sort_export.c    ******/
int		print_sorted_env(t_data *data);

/******    env_utils.c    ******/
char	*get_env_value(t_list *env_list, char *key);
t_list	*find_env_node(t_list *env, char *key);
int		update_or_add_env(t_list **env, char *key, char *value);
void	remove_env_node(t_list **env, char *key);

/******    env.c    ******/
t_list	*init_env_list(char **env);
char	**copy_env(char **env);
int		sync_env(t_data *data);
char	**env_list_to_array(t_list *env_list);

/******    child.c   ******/
void	exec_child(t_cmd *cmd, t_data *data, int prev_read_fd);

/******    cleanup_utils.c    ******/
void	close_all_pipes(t_cmd *cmd_list);
void	close_cmd_fds(t_cmd *cmd);
void	cleanup_heredocs(t_data *data);
void	cleanup_child(t_data *data);

/******    heredoc.c   ******/
int		check_heredoc(t_data *data);

/******    path.c   ******/
char	*get_full_path(char *cmd, t_list *env_list);
void	init_cmd_path(t_cmd *cmd, t_data *data);

/******    pipe_utils.c    ******/
int		set_pipe(t_cmd *cmd);
void	handle_redir_fds(t_cmd *cmd);
void	connect_pipes(t_cmd *cmd, int prev_pipe_read);

/******    pipeline.c    ******/
void	execute_pipeline(t_data *data);

/******    wait.c    ******/
void	wait_all_children(t_data *data);

/******    signals.c    ******/
void	ignore_signals(void);
void	set_signal_action(void);

t_cmd		*cmd_new(void);
t_cmd		*cmd_last(t_cmd *lst);
void		cmd_add_back(t_cmd **lst, t_cmd *new);
void		cmd_clear(t_cmd **lst);
void		free_cycle(t_data *data);
void		free_data(t_data *data);

t_redir		*redir_new(t_redir_type type, char *filename);
t_redir		*redir_last(t_redir *lst);
void		redir_add_back(t_redir **lst, t_redir *new);
void		redir_clear(t_redir **lst);
t_redir		*create_redir(t_token *token, char *filename);

/* utils_to_delete.c */
void		print_cmds(t_cmd *cmds);
void		print_tokens(t_token *tokens);

/* parser.c */
t_cmd		*parser(t_token *tokens);

/* token_utils.c */
t_token	*token_new(char *value, int type);
t_token	*token_last(t_token *token);
void	token_add_back(t_token **lst, t_token *new);
void	token_add_front(t_token **lst, t_token *new);
int		token_size(t_token *lst);
void	token_clear(t_token **lst);

/* lexer_utils.c */
bool	is_whitespace(char c);
bool	is_operator(char c);
void	skip_whitespace(char *line, int *i);

/* expander_utils.c */
int		get_var_len(char *str);
char	*char_append(char *s, char c);
char	*get_env_value_tab(char *var, char **env);

int	lexer(char *line, t_token **tokens);
int	expander(t_token *tokens, struct s_data *data);

/* parser_utils.c */
int		ft_arrlen(char **arr);
char	**ft_append_str(char **arr, char *str);

/* errors.c */
void	*ms_error(char *err_msg, void *to_free);
void	synterr(t_token *token, char c, bool nl, t_data *data);
void	eoferr(t_state state, t_data *data);

/* check_syntax.c */
int	check_syntax(t_token *tokens, t_data *data);

#endif
