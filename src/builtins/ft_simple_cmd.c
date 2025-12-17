/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_simple_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:14:45 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/17 18:50:01 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	n_option;

	i = 1;
	n_option = 0;
	while (args[i] && ft_strncmp(args[i], "-n", 3) == 0)
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
		free(cwd);
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
