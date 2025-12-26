/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 13:21:24 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/26 13:23:49 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_valid_unset_key(char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		// Si on trouve un caractère non-alnum (y compris '='), c'est faux.
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/*
** Builtin unset command.
** Removes environment variables specified as arguments.
** @param args: The args array (args[0] = "unset", args[1+] = vars to remove).
** @param data: Global data structure.
** @return: Always returns 0 (success).
*/
int	builtin_unset(char **args, t_data *data)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	while (args[i])
	{
		if (!is_valid_unset_key(args[i]))
		{
			print_error_var("unset", args[i], "not a valid identifier");
			ret = 1;
		}
		else
			remove_env_node(&data->env_list, args[i]);
		i++;
	}
	sync_env(data);
	return (ret);
}
