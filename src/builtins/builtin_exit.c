/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 12:18:53 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/22 18:03:50 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Checks if a string contains only digits (with optional leading +/-).
** Also checks if the string is not empty after the sign.
** @param str: The string to check.
** @return: 1 if numeric, 0 otherwise.
*/
static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/*
** Converts a string to long long, detecting overflow.
** @param str: The string to convert.
** @param overflow: Pointer to set to 1 if overflow occurs.
** @return: The converted value, or 0 if overflow.
*/
static long long	ft_atoll_safe(char *str, int *overflow)
{
	long long	res;
	int			sign;
	int			i;

	res = 0;
	sign = 1;
	i = 0;
	*overflow = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		if (res > (LLONG_MAX - (str[i] - '0')) / 10)
		{
			*overflow = 1;
			return (0);
		}
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

/*
** Prints numeric argument error.
** @param arg: The invalid argument.
*/
static void	print_numeric_error(char *arg)
{
	ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd(": numeric argument required", STDERR_FILENO);
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
		print_numeric_error(arg);
		return (2);
	}
	code = ft_atoll_safe(arg, &overflow);
	if (overflow)
	{
		print_numeric_error(arg);
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

	ft_putstr_fd("exit\n", STDOUT_FILENO);
	if (!args[1])
		exit_code = data->last_exit_code;
	else if (args[2] && is_numeric(args[1]))
	{
		ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
		return (1);
	}
	else
		exit_code = parse_exit_arg(args[1]);
	ft_lstclear(&data->env_list, free);
	if (data->env)
		free(data->env);
	if (data->cmd_list)
		cmd_clear(&data->cmd_list);
	if (data->line)
		free(data->line);
	rl_clear_history();
	if (data->stdin_backup != -1)
		close(data->stdin_backup);
	if (data->stdout_backup != -1)
		close(data->stdout_backup);
	exit((unsigned char)exit_code);
}
