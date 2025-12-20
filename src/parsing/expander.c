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
** Helper: Handles variable expansion.
** Returns updated string and advances index i past the variable name.
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
** Updates quote state and returns 1 if quote should be removed.
** Returns 0 if the quote is inside another quote type (preserved).
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

char	*expand_token(char *t, t_data *d)
{
	char	*res;
	int		i;
	t_state	state;

	i = 0;
	state = STATE_IDLE;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (t[i])
	{
		if (is_quote_toggle(t[i], &state))
			;
		else if (t[i] == '$' && state != STATE_QUOTES)
			res = handle_expansion(res, t, &i, d);
		else if (ft_strchr("\\;&", t[i]))
			return (synterr(NULL, *ft_strchr("\\;&", t[i]), 0, d), free(res), NULL);
		else
			res = char_append(res, t[i]);
		i++;
	}
	if (state != STATE_IDLE)
		return (eoferr(state, d), free(res), NULL);
	return (res);
}

int	expander(t_token *tokens, t_data *data)
{
	char	*new;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			new = expand_token(tokens->value, data);
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
