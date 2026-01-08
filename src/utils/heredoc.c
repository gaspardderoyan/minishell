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
