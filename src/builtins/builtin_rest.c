/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_rest.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:14:45 by mgregoir          #+#    #+#             */
/*   Updated: 2026/01/07 15:53:52 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

	if (!args)
		return (1);
	i = 1;
	n_option = 0;
	while (args[i] && is_n_flag(args[i]))
	{
		n_option = 1;
		i++;
	}
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_option)
		printf("\n");
	return (0);
}

/*
** Builtin pwd command.
** Prints the current working directory to stdout.
** @return: 0 on success, 1 on error.
*/
int	builtin_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)))
	{
		printf("%s\n", cwd);
		return (0);
	}
	perror("minishell: pwd");
	return (1);
}

/*
** Builtin env command.
** Prints all environment variables containing '=' to stdout.
** @param env: The environment linked list.
** @return: Always returns 0 (success).
*/
int	builtin_env(t_list *env)
{
	while (env)
	{
		if (ft_strchr((char *)env->content, '='))
			printf("%s\n", (char *)env->content);
		env = env->next;
	}
	return (0);
}
