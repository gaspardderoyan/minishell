/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 10:25:14 by gderoyan          #+#    #+#             */
/*   Updated: 2026/01/08 16:45:50 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Generates a unique temporary filename for a heredoc.
** @param i: The unique index or identifier for this heredoc.
** @return: A malloc'd string containing the filename (e.g., ".heredoc_tmp_0"),
** or NULL if allocation fails.
*/
char	*generate_heredoc_name(int i)
{
	char	*num;
	char	*name;

	num = ft_itoa(i);
	if (!num)
		return (NULL);
	name = ft_strjoin(".heredoc_tmp_", num);
	free(num);
	return (name);
}

/*
** Handles the interruption of the heredoc input loop (via SIGINT/Ctrl+C).
** Restores the original STDIN from the backup fd and updates the exit code.
** @param data: The main data structure.
** @param stdin_backup: The file descriptor of the saved STDIN.
** @return: Always returns 1 to indicate failure/interruption to the caller.
*/
int	handle_heredoc_interrupt(t_data *data, int stdin_backup)
{
	if (g_status == 130)
	{
		dup2(stdin_backup, STDIN_FILENO);
		data->last_exit_code = 130;
	}
	else
		data->last_exit_code = 1;
	close(stdin_backup);
	set_signal_action();
	return (1);
}

/*
** Retrieves a variable's value and appends it to the result string.
** Handles the special "$?" variable and standard environment variables.
** @param res: The current result string.
** @param name: The name of the variable to look up.
** @param data: The main data structure containing the environment.
** @return: The new result string with the value appended.
*/
static char	*append_var_value(char *res, char *name, t_data *data)
{
	char	*val;
	char	*tmp;

	if (ft_strncmp(name, "?", 2) == 0)
		val = ft_itoa(data->last_exit_code);
	else
		val = get_env_value_tab(name, data->env);
	tmp = res;
	res = ft_strjoin(res, val);
	free(tmp);
	free(val);
	return (res);
}

/*
** Processes a '$' character found in the line.
** Extracts the variable name length, and either appends a literal '$'
** (if no valid name follows) or expands the variable.
** @param res: Address of the result string pointer.
** @param line: The original line being parsed.
** @param i: Pointer to the current index in 'line'.
** @param data: The main data structure.
*/
static void	append_dollar(char **res, char *line, int *i, t_data *data)
{
	int		len;
	char	*name;

	len = get_var_len(&line[*i + 1]);
	if (len == 0)
	{
		*res = char_append(*res, '$');
		(*i)++;
	}
	else
	{
		name = ft_substr(line, *i + 1, len);
		*res = append_var_value(*res, name, data);
		free(name);
		*i += len + 1;
	}
}

/*
** Expands environment variables within a line read from heredoc.
** Iterates through the line, handling '$' expansions and copying
** literal characters.
** @param line: The raw line read from readline.
** @param data: The main data structure.
** @return: A new allocated string with variables expanded.
*/
char	*expand_heredoc_line(char *line, t_data *data)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			append_dollar(&res, line, &i, data);
		else
			res = char_append(res, line[i++]);
	}
	return (res);
}
