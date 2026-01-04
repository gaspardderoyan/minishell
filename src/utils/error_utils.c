/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 20:00:00 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/26 13:14:59 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Prints a formatted error message to stderr.
** Format: "minishell: [cmd: ][arg: ]msg\n"
** @param cmd: Optional command name (can be NULL).
** @param arg: Optional argument that caused the error (can be NULL).
** @param msg: The error message to display.
*/
void	print_error(char *cmd, char *arg, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (arg)
	{
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
}

/*
** Prints a formatted error message for variable-related errors.
** Format: "minishell: [cmd: ]`arg': msg\n"
** Used specifically for export/unset variable validation errors.
** @param cmd: Optional command name (can be NULL).
** @param arg: Variable name that caused the error (printed with backticks).
** @param msg: The error message to display.
*/
void	print_error_var(char *cmd, char *arg, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (arg)
	{
		ft_putstr_fd("`", STDERR_FILENO);
		ft_putstr_fd(arg, STDERR_FILENO);
		ft_putstr_fd("'", STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd(msg, STDERR_FILENO);
}
