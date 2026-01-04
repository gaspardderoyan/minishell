/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 15:42:39 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/16 15:43:02 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Checks if a character is whitespace.
** Includes spaces, tabs, and newlines.
** @param c: Character to check.
** @return: true if whitespace, false otherwise.
*/
bool	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (true);
	return (false);
}

/*
** Checks if a character is a shell operator.
** Recognizes: | (pipe), > (output), < (input).
** @param c: Character to check.
** @return: true if operator, false otherwise.
*/
bool	is_operator(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (true);
	return (false);
}

/*
** Skips whitespace characters in a string by advancing the index.
** @param line: The string to parse.
** @param i: Pointer to the current index (will be updated).
*/
void	skip_whitespace(char *line, int *i)
{
	while (is_whitespace(line[*i]) && line[*i])
		(*i)++;
}
