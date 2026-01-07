/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 20:00:00 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/26 13:14:59 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <unistd.h>

static char	*str_append(char *s1, char *s2)
{
	char	*new_str;

	if (!s1)
		return (NULL);
	if (!s2)
		return (s1);
	new_str = ft_strjoin(s1, s2);
	free(s1);
	return (new_str);
}

/*
** Prints a formatted error message to stderr.
** Format: "minishell: [cmd: ][arg: ]msg\n"
** @param cmd: Optional command name (can be NULL).
** @param arg: Optional argument that caused the error (can be NULL).
** @param msg: The error message to display.
*/
void	print_error(char *cmd, char *arg, char *msg)
{
	char	*str;

	str = ft_strdup("minishell: ");
	if (!str)
		return ;
	if (cmd)
	{
		str = str_append(str, cmd);
		str = str_append(str, ": ");
	}
	if (arg)
	{
		str = str_append(str, arg);
		str = str_append(str, ": ");
	}
	str = str_append(str, msg);
	str = str_append(str, "\n");
	if (str)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		free(str);
	}
}

/*
** Prints a formatted error message for invalid variable identifiers.
** Format: "minishell: [cmd: ]`arg': msg\n"
** @param cmd: Optional command name (can be NULL).
** @param arg: The invalid argument (e.g., variable name).
** @param msg: The error message to display.
*/
void	print_error_var(char *cmd, char *arg, char *msg)
{
	char	*str;

	str = ft_strdup("minishell: ");
	if (!str)
		return ;
	if (cmd)
	{
		str = str_append(str, cmd);
		str = str_append(str, ": ");
	}
	if (arg)
	{
		str = str_append(str, "`");
		str = str_append(str, arg);
		str = str_append(str, "': ");
	}
	str = str_append(str, msg);
	str = str_append(str, "\n");
	if (str)
	{
		ft_putstr_fd(str, STDERR_FILENO);
		free(str);
	}
}

/*
** Handles the warning message when EOF (Ctrl+D) is encountered
** during heredoc input.
** Checks if interruption was by signal (Ctrl+C) first.
** @param delimiter: The expected delimiter string.
** @param line_count: The line number where heredoc started.
** @return: -1 if interrupted by signal, 0 if true EOF.
*/
int	print_eof_warning(char *delim, int line_count)
{
	char	*msg;
	char	*s_cnt;

	if (g_status == 130)
		return (-1);
	msg = ft_strdup("minishell: warning: here-document at line ");
	if (!msg)
		return (-1);
	s_cnt = ft_itoa(line_count);
	if (!s_cnt)
		return (free(msg), -1);
	msg = str_append(msg, s_cnt);
	free(s_cnt);
	msg = str_append(msg, " delimited by end-of-file (wanted `");
	msg = str_append(msg, delim);
	msg = str_append(msg, "')\n");
	write(STDERR_FILENO, msg, ft_strlen(msg));
	free(msg);
	return (0);
}
