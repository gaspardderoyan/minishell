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

#include "minishell.h"

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
