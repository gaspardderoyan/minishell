/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 10:25:14 by gderoyan          #+#    #+#             */
/*   Updated: 2026/01/07 10:25:14 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*generate_heredoc_name(int i)
{
	char	*num;
	char	*name;

	num = ft_itoa(i);
	if (!num)
		return (NULL);
	name = ft_strjoin(".heredoc_tmp_", num);
	free(num);
	return (name);
}

int	handle_heredoc_interrupt(t_data *data, int stdin_backup)
{
	if (g_status == 130)
	{
		dup2(stdin_backup, STDIN_FILENO);
		data->last_exit_code = 130;
	}
	else
		data->last_exit_code = 1;
	close(stdin_backup);
	set_signal_action();
	return (1);
}

static char	*append_var_value(char *res, char *name, t_data *data)
{
	char	*val;
	char	*tmp;

	if (ft_strncmp(name, "?", 2) == 0)
		val = ft_itoa(data->last_exit_code);
	else
		val = get_env_value_tab(name, data->env);
	tmp = res;
	res = ft_strjoin(res, val);
	free(tmp);
	free(val);
	return (res);
}

static void	append_dollar(char **res, char *line, int *i, t_data *data)
{
	int		len;
	char	*name;

	len = get_var_len(&line[*i + 1]);
	if (len == 0)
	{
		*res = char_append(*res, '$');
		(*i)++;
	}
	else
	{
		name = ft_substr(line, *i + 1, len);
		*res = append_var_value(*res, name, data);
		free(name);
		*i += len + 1;
	}
}

char	*expand_heredoc_line(char *line, t_data *data)
{
	char	*res;
	int		i;

	res = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
			append_dollar(&res, line, &i, data);
		else
			res = char_append(res, line[i++]);
	}
	return (res);
}
