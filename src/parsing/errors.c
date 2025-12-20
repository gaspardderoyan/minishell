/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:29:58 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/17 18:30:00 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ms_error(char *err_msg, void *to_free)
{
	if (to_free)
		free(to_free);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (NULL);
}

/*
** prints to STDERR the 'minishell: syntax error near unexpected token X' message
** where X will be 'newline' if nl is TRUE
** else if c != 0, X = c
** else c = token->value[0]
*/
void	synterr(t_token *token, char c, bool nl, t_data *data)
{
	int		fn;

	fn = STDERR_FILENO;
	data->last_exit_code = 2;
	if (nl)
	{
		ft_dprintf(fn, "minishell: syntax error near unexpected token `newline'\n");
		return ;
	}
	if (!c)
		c = *token->value;
	ft_dprintf(fn, "minishell: syntax error near unexpected token `%c'\n", c);
}

void	eoferr(t_state state)
{
	int		fn;
	char	c;

	fn = STDERR_FILENO;
	c = '\'';
	if (state == STATE_DQUOTES)
		c = '"';
	ft_dprintf(fn, "minishell: unexpected EOF while ");
	ft_dprintf(fn, "looking for matching `%c'\n", c);
}
