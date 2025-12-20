/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_rest.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:14:45 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/19 16:48:09 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
** Checks if arg is a valid -n flag (handles -n, -nn, -nnn, etc.).
** @param arg: The argument to check.
** @return: 1 if valid -n flag, 0 otherwise.
*/
static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 1;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

/*
** Builtin echo command.
** Prints arguments to stdout, -n flag suppresses trailing newline.
** @param args: The argument array.
** @return: Always returns 0 (success).
*/
int	builtin_echo(char **args)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = 0;
	while (args[i] && is_n_flag(args[i]))
	{
		n_option = 1;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
			ft_putstr_fd(" ", STDOUT_FILENO);
		i++;
	}
	if (!n_option)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (0);
}

int	builtin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		ft_putendl_fd(cwd, STDOUT_FILENO);
		return (0);
	}
	perror("minishell: pwd");
	return (1);
}

int	builtin_env(t_list *env)
{
	while (env)
	{
		if (ft_strchr((char *)env->content, '='))
			ft_putendl_fd((char *)env->content, STDOUT_FILENO);
		env = env->next;
	}
	return (0);
}

int	builtin_unset(char **args, t_data *data)
{
	int	i;

	i = 1;
	while (args[i])
	{
		remove_env_node(&data->env_list, args[i]);
		i++;
	}
	return (0);
}
