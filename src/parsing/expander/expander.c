/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:09:00 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/17 18:29:54 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Retrieves the value of a variable from the environment.
** Handles special variable "$?" (exit code).
** @param token: The token string containing the variable.
** @param i: Index of the '$' character in the token.
** @param len: Length of the variable name.
** @param data: Global data structure containing environment and exit code.
** @return: The variable value, or NULL if not found.
*/
static char	*get_var_val(char *token, int i, int len, t_data *data)
{
	char	*var_name;
	char	*var_val;

	var_name = ft_substr(token, i + 1, len);
	if (!var_name)
		return (NULL);
	if (ft_strncmp(var_name, "?", 2) == 0)
		var_val = ft_itoa(data->last_exit_code);
	else
		var_val = get_env_value_tab(var_name, data->env);
	free(var_name);
	return (var_val);
}

/*
** Handles variable expansion in a token.
** Expands "$VAR" to its value, advances index past the variable name.
** @param res: The result string being built.
** @param token: The token string being processed.
** @param i: Pointer to current index (will be updated).
** @param data: Global data structure for environment/exit code.
** @return: Updated result string with expansion, or NULL on error.
*/
static char	*handle_expansion(char *res, char *token, int *i, t_data *data)
{
	char	*var;
	char	*temp;
	int		len;

	len = get_var_len(&token[*i + 1]);
	if (len == 0)
	{
		res = char_append(res, '$');
		if (!res)
			return (NULL);
		return (res);
	}
	var = get_var_val(token, *i, len, data);
	temp = res;
	res = ft_strjoin(res, var);
	if (!res)
		return (free(temp), free(var), NULL);
	*i += len;
	return (free(temp), free(var), res);
}

/*
** Toggles quote state when encountering quotes.
** @param c: Current character ('\'' or '"').
** @param state: Pointer to current quote state.
** @return: 1 if quote should be removed, 0 if preserved (nested).
*/
static int	is_quote_toggle(char c, t_state *state)
{
	if (c == '\'' && *state != STATE_DQUOTES)
	{
		if (*state == STATE_QUOTES)
			*state = STATE_IDLE;
		else
			*state = STATE_QUOTES;
		return (1);
	}
	if (c == '\"' && *state != STATE_QUOTES)
	{
		if (*state == STATE_DQUOTES)
			*state = STATE_IDLE;
		else
			*state = STATE_DQUOTES;
		return (1);
	}
	return (0);
}

/*
** Expands variables and removes quotes from a token string.
** Handles quotes, variable expansion, and forbidden characters.
** @param tkn: The token string to expand.
** @param d: Global data structure.
** @return: The expanded string, or NULL on error (syntax/memory).
*/
static char	*expand_token(t_token *token, t_data *d)
{
	char	*res;
	int		i;
	t_state	state;
	char	*tkn;

	i = 0;
	tkn = token->value;
	state = STATE_IDLE;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (tkn[i])
	{
		if (is_quote_toggle(tkn[i], &state))
			token->quoted = 1;
		else if (tkn[i] == '$' && state != STATE_QUOTES)
			res = handle_expansion(res, tkn, &i, d);
		else
			res = char_append(res, tkn[i]);
		i++;
	}
	if (state != STATE_IDLE)
		return (eoferr(state, d), free(res), NULL);
	return (res);
}

/*
** Main expander function. Expands all TOKEN_WORD tokens in the list.
** @param tokens: The head of the token linked list.
** @param data: Global data structure.
** @return: SUCCESS (0) on success, FAIL (1) on error.
*/
int	expander(t_token *tokens, t_data *data)
{
	char	*new;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			new = expand_token(tokens, data);
			free(tokens->value);
			tokens->value = NULL;
			if (!new)
				return (FAIL);
			tokens->value = new;
		}
		tokens = tokens->next;
	}
	return (SUCCESS);
}
