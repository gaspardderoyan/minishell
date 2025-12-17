/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 16:09:00 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/16 16:18:09 by gderoyan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"
#include <unistd.h>

static void	*ms_error(char *err_msg, void *to_free)
{
	if (to_free)
		free(to_free);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (NULL);
}

/*
** Helper: Handles variable expansion.
** Returns updated string and advances index i past the variable name.
*/
static char	*handle_expansion(char *res, char *token, int *i, char **env)
{
	char	*var_name;
	char	*var_val;
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
	var_name = ft_substr(token, *i + 1, len);
	if (!var_name)
		return (NULL);
	var_val = get_env_value_tab(var_name, env);
	if (!var_val)
		return (free(var_name), NULL);
	temp = res;
	res = ft_strjoin(res, var_val);
	if (!res)
		return (free(temp), free(var_val), free(var_name), NULL);
	*i += len;
	return (free(temp), free(var_val), free(var_name), res);
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

char	*expand_token(char *token, char **env)
{
	char	*res;
	int		i;
	t_state	state;

	i = 0;
	state = STATE_IDLE;
	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (token[i])
	{
		if (is_quote_toggle(token[i], &state))
			;
		else if (token[i] == '$' && state != STATE_QUOTES)
			res = handle_expansion(res, token, &i, env);
		else if (ft_strchr("\\;&", token[i]))
			return (ms_error("unexpected token", res));
		else
			res = char_append(res, token[i]);
		i++;
	}
	if (state != STATE_IDLE)
		return (ms_error("unexpected EOF", res));
	return (res);
}

int	expander(t_token *tokens, char **env)
{
	char	*new;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			new = expand_token(tokens->value, env);
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
