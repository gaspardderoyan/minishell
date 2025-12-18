/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:05:07 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/18 16:07:29 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_cd(char **args, t_data *data)
{
	char	cwd[1024];
	char	*path;

	if (!args[1])
	{
		// Pour faire propre, il faudrait utiliser get_env_value
		return (0); 
	}
	path = args[1];
	getcwd(cwd, sizeof(cwd));
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		perror(path);
		return (1);
	}
	update_or_add_env(&data->env_list, "OLDPWD", cwd);
	if (getcwd(cwd, sizeof(cwd)))
		update_or_add_env(&data->env_list, "PWD", cwd);
	return (0);
}
