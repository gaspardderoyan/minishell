/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 12:18:53 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/23 16:34:05 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Checks if a string contains only digits (with optional leading +/-).
** Allows leading and trailing whitespace (bash behavior).
** @param str: The string to check.
** @return: 1 if numeric, 0 otherwise.
*/
static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i] && ft_isdigit(str[i]))
		i++;
	while (str[i])
	{
		if (!ft_isspace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
** Parses sign and returns index after sign.
** @param str: The string to parse.
** @param sign: Pointer to store the sign (-1 or 1).
** @return: Index after the sign.
*/
static int	parse_sign(char *str, int *sign)
{
	int	i;

	i = 0;
	*sign = 1;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			*sign = -1;
		i++;
	}
	return (i);
}

/*
** Converts a string to long long, detecting overflow.
** Accumulates in negative to handle LLONG_MIN correctly.
** @param str: The string to convert.
** @param overflow: Pointer to set to 1 if overflow occurs.
** @return: The converted value, or 0 if overflow.
*/
static long long	ft_atoll_safe(char *str, int *overflow)
{
	long long	res;
	int			sign;
	int			i;
	int			digit;

	res = 0;
	*overflow = 0;
	i = parse_sign(str, &sign);
	while (str[i] && ft_isdigit(str[i]))
	{
		digit = str[i] - '0';
		if (res < (LLONG_MIN + digit) / 10)
		{
			*overflow = 1;
			return (0);
		}
		res = res * 10 - digit;
		i++;
	}
	if (sign == 1 && res == LLONG_MIN)
		*overflow = 1;
	if (sign == 1)
		return (-res);
	return (res);
}

/*
** Parses the exit argument and returns the exit code.
** @param arg: The argument string.
** @return: The exit code (2 if invalid/overflow).
*/
static long long	parse_exit_arg(char *arg)
{
	long long	code;
	int			overflow;

	if (!is_numeric(arg))
	{
		print_error("exit", arg, "numeric argument required");
		return (2);
	}
	code = ft_atoll_safe(arg, &overflow);
	if (overflow)
	{
		print_error("exit", arg, "numeric argument required");
		return (2);
	}
	return (code);
}

/*
** Builtin exit command.
** Exits the shell with the specified exit code.
** @param args: The argument array.
** @param data: Global data structure.
** @return: 1 if too many args (doesn't exit), otherwise exits.
*/
int	builtin_exit(char **args, t_data *data)
{
	long long	exit_code;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (!args[1])
		exit_code = data->last_exit_code;
	else if (args[2] && is_numeric(args[1]))
	{
		print_error("exit", NULL, "too many arguments");
		return (1);
	}
	else
		exit_code = parse_exit_arg(args[1]);
	cleanup_exit(data);
	exit((unsigned char)exit_code);
}
