/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 19:12:51 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/23 18:20:57 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Validates an environment variable key name.
** Key must start with letter or underscore, followed by alphanumeric or _.
** @param key: The key string to validate (e.g., "VAR_NAME" or "VAR_NAME=value")
** @return: 1 if valid, 0 otherwise.
*/
static int	is_valid_env_key(char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/*
** Processes a single export argument (KEY or KEY=VALUE).
** @param arg: The argument string.
** @param data: Global data structure.
** @return: 0 on success, 1 on error.
*/
static int	process_export_arg(char *arg, t_data *data)
{
	char	*equal_sign;
	char	*value;

	if (!is_valid_env_key(arg))
	{
		ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
		return (1);
	}
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		value = equal_sign + 1;
		if (update_or_add_env(&data->env_list, arg, value) == -1)
			return (1);
		*equal_sign = '=';
	}
	return (0);
}

/*
** Builtin export command.
** Without args: prints all env vars. With args: sets variables.
** @param args: The argument array.
** @param data: Global data structure.
** @return: 0 on success, 1 on error.
*/
int	builtin_export(char **args, t_data *data)
{
	int	i;
	int	ret;

	if (!args[1])
		return (print_sorted_env(data));
	ret = 0;
	i = 1;
	while (args[i])
	{
		if (process_export_arg(args[i], data) != 0)
			ret = 1;
		i++;
	}
	sync_env(data);
	return (ret);
}
