/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 17:58:43 by mgregoir          #+#    #+#             */
/*   Updated: 2026/01/05 12:10:33 by mgregoir         ###   ########.fr       */
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

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

# define CMD_NOT_FOUND 127
# define CANT_EXEC 126

# define SUCCESS 0
# define FAIL 1

extern volatile sig_atomic_t	g_status;

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
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}			t_redir_type;

/* -------------------------------------------------------
   2. REDIRECTION LIST
   Stores a list of files/delimiters associated with a command.
------------------------------------------------------- */
typedef struct s_redir
{
	t_redir_type	type;
	char			*filename;
	struct s_redir	*next;
}		t_redir;

/* -------------------------------------------------------
   3. COMMAND TABLE
   One node per command. A pipeline is a linked list of these.
------------------------------------------------------- */
typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
	char			*cmd_path;
	pid_t			pid;
	int				fd_in;
	int				fd_out;
	int				pipefd[2];
	char			*heredoc_file;
}			t_cmd;

typedef struct s_data
{
	t_cmd	*cmd_list;
	char	**env;
	t_list	*env_list;
	int		last_exit_code;
	t_token	*tokens;
	char	*line;
	int		stdin_backup;
	int		stdout_backup;
	int		line_count;
}	t_data;

/******  BUILTINS - builtin_cd.c  ******/
int			builtin_cd(char **args, t_data *data);

/******  BUILTINS - builtin_exit.c  ******/
int			builtin_exit(char **args, t_data *data);

/******  BUILTINS - builtin_export.c  ******/
int			builtin_export(char **args, t_data *data);

/******  BUILTINS - builtin_rest.c  ******/
int			builtin_echo(char **args);
int			builtin_pwd(void);
int			builtin_env(t_list *env);

/******  BUILTINS - builtin_unset.c  ******/
int			builtin_unset(char **args, t_data *data);

/******  BUILTINS - builtins_dispatch.c  ******/
int			is_builtin(char *cmd);
int			is_modifier_builtin(char *cmd);
int			dispatch_builtin(t_cmd *cmd, t_data *data);

/******  BUILTINS - builtins_exec.c  ******/
int			apply_redirections(t_cmd *cmd);
void		execute_builtin_in_parent(t_cmd *cmd, t_data *data);

/******  BUILTINS - sort_export.c  ******/
int			print_sorted_env(t_data *data);

/******  ENV - env.c  ******/
t_list		*init_env_list(char **env);
char		**copy_env(char **env);
int			sync_env(t_data *data);
char		**env_list_to_array(t_list *env_list);

/******  EXEC - child.c  ******/
void		exec_child(t_cmd *cmd, t_data *data, int prev_read_fd);

/******  EXEC - heredoc.c  ******/
int			check_heredoc(t_data *data);
char		*generate_heredoc_name(int i);
int			handle_heredoc_interrupt(t_data *data, int stdin_backup);

/******  EXEC - pipeline.c  ******/
void		execute_pipeline(t_data *data);

/******  EXEC - wait.c  ******/
void		wait_all_children(t_data *data);

/******  PARSING - EXPANDER - expander.c  ******/
int			expander(t_token *tokens, struct s_data *data);

/******  PARSING - EXPANDER - expander_helpers.c  ******/
int			get_var_len(char *str);
char		*char_append(char *s, char c);
char		*get_env_value_tab(char *var, char **env);	

/******  PARSING - LEXER - lexer.c  ******/
int			lexer(char *line, t_token **tokens);

/******  PARSING - LEXER - lexer_helpers.c  ******/
bool		is_whitespace(char c);
bool		is_operator(char c);
void		skip_whitespace(char *line, int *i);

/******  PARSING - LEXER - token.c  ******/
t_token		*token_new(char *value, int type);
t_token		*token_last(t_token *token);
void		token_add_back(t_token **lst, t_token *new);
void		token_clear(t_token **lst);
void		token_delete(t_token **lst, t_token *token);

/******  PARSING - PARSER - command.c  ******/
t_cmd		*cmd_new(void);
void		cmd_add_back(t_cmd **lst, t_cmd *new);
void		cmd_clear(t_cmd **lst);

/******  PARSING - PARSER - parser_helpers.c  ******/
char		**ft_append_str(char **arr, char *str);
t_redir		*create_redir(t_token *token, char *filename);

/******  PARSING - PARSER - parser.c  ******/
t_cmd		*parser(t_token *tokens);

/******  PARSING - PARSER - redirection.c  ******/
t_redir		*redir_new(t_redir_type type, char *filename);
void		redir_add_back(t_redir **lst, t_redir *new);
void		redir_clear(t_redir **lst);

/******  PARSING - syntax.c  ******/
void		remove_empty_tokens(t_token **tokens);
int			check_syntax(t_token *tokens, t_data *data);

/******  PARSING - errors.c  ******/
void		*ms_error(char *err_msg, void *to_free);
void		synterr(t_token *token, char c, bool nl, t_data *data);
void		eoferr(t_state state, t_data *data);

/******  UTILS - signals.c  ******/
void		ignore_signals(void);
void		set_signal_action(void);
void		reset_signals_default(void);
void		set_signal_heredoc(void);

/******  UTILS - cleanup.c  ******/
void		close_all_pipes(t_cmd *cmd_list);
void		cleanup_heredocs(t_data *data);
void		cleanup_child(t_data *data);
void		cleanup_exit(t_data *data);

/******  UTILS - env.c  ******/
char		*get_env_value(t_list *env_list, char *key);
t_list		*find_env_node(t_list *env, char *key);
int			update_or_add_env(t_list **env, char *key, char *value);
void		remove_env_node(t_list **env, char *key);

/******  UTILS - errors.c  ******/
void		print_error(char *cmd, char *arg, char *msg);
void		print_error_var(char *cmd, char *arg, char *msg);

/******  UTILS - path.c  ******/
char		*get_full_path(char *cmd, t_list *env_list);
void		init_cmd_path(t_cmd *cmd, t_data *data);

/******  UTILS - pipe.c  ******/
int			set_pipe(t_cmd *cmd);
void		handle_redir_fds(t_cmd *cmd);
void		connect_pipes(t_cmd *cmd, int prev_pipe_read);

/******  UTILS - memory.c  ******/
void		init_data(t_data *data, char **env, int *ac, char ***av);
void		free_cycle(t_data *data);
void		free_data(t_data *data);

#endif
