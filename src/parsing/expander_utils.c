/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:10:14 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/16 16:11:27 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_var_len(char *str)
{
	int	i;

	if (*str == '?')
		return (1);
	else if (ft_isdigit(*str))
		return (1);
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (i);
}

char	*char_append(char *s, char c)
{
	char	*new_s;
	int		len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	new_s = malloc(sizeof(char) * (len + 2));
	if (!new_s)
		return (free(s), NULL);
	ft_strlcpy(new_s, s, len + 1);
	new_s[len] = c;
	new_s[len + 1] = 0;
	return (free(s), new_s);
}

char	*get_env_value_tab(char *var, char **env)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (ft_strncmp(env[i], var, len) == 0 && env[i][len] == '=')
			return (ft_strdup(&env[i][len + 1]));
		i++;
	}
	return (ft_strdup(""));
}
