/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_unset.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 19:12:51 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/17 20:32:29 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

int	builtin_export(char **args, t_data *data)
{
	int		i;
	char	*key;
	char	*value;
	char	*equal_sign;

	if (!args[1])
		return (builtin_env(data->env_list));
	i = 1;
	while (args[i])
	{
		if (!is_valid_env_key(args[i]))
		{
			ft_putstr_fd("minishell: export: not a valid identifier\n", 2);
			return (1);
		}
		equal_sign = ft_strchr(args[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			key = args[i];
			value = equal_sign + 1;
			update_or_add_env(&data->env_list, key, value);
			*equal_sign = '=';
		}
		i++;
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
