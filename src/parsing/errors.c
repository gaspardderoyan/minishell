/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:29:58 by gderoyan          #+#    #+#             */
/*   Updated: 2026/01/07 17:37:20 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Prints an error message and frees the provided pointer.
** @param err_msg: The error message to display.
** @param to_free: Pointer to free (can be NULL).
** @return: Always returns NULL (for convenient error propagation).
*/
void	*ms_error(char *err_msg, void *to_free)
{
	if (to_free)
		free(to_free);
	print_error(NULL, NULL, err_msg);
	return (NULL);
}

/*
** Prints a syntax error message to stderr.
** Formats: "minishell: syntax error near unexpected token `X'"
** @param token: The token causing the error (can be NULL).
** @param c: Character to display (0 = use token->value[0]).
** @param nl: If true, displays "newline" instead of a character.
** @param data: Global data structure (sets exit code to 2).
*/
void	synterr(t_token *token, char c, bool nl, t_data *data)
{
	data->last_exit_code = 2;
	if (nl)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `newline'\n");
		return ;
	}
	if (!c)
		c = *token->value;
	ft_dprintf(STDERR_FILENO,
		"minishell: syntax error near unexpected token `%c'\n", c);
}

/*
** Prints an error message for unexpected EOF while parsing quotes.
** @param state: The current parsing state (STATE_QUOTES or STATE_DQUOTES).
** @param data: Global data structure (sets exit code to 2).
*/
void	eoferr(t_state state, t_data *data)
{
	char	c;

	data->last_exit_code = 2;
	c = '\'';
	if (state == STATE_DQUOTES)
		c = '"';
	ft_dprintf(STDERR_FILENO,
		"minishell: unexpected EOF while looking for matching `%c'\n", c);
}
