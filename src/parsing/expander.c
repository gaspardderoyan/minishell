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

static char	*char_append(char *s, char c)
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

static char	*get_env_value(char *var, char **env)
{
	int		i;
	int		len;

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
		return (res);
	}
	var_name = ft_substr(token, *i + 1, len);
	var_val = get_env_value(var_name, env);
	temp = res;
	res = ft_strjoin(res, var_val);
	free(temp);
	free(var_name);
	free(var_val);
	*i += len;
	return (res);
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
	while (token[i])
	{
		if (is_quote_toggle(token[i], &state))
			;
		else if (token[i] == '$' && state != STATE_QUOTES)
			res = handle_expansion(res, token, &i, env);
		else
			res = char_append(res, token[i]);
		i++;
	}
	return (res);
}

void	expander(t_token *tokens, char **env)
{
	char	*new;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD)
		{
			new = expand_token(tokens->value, env);
			free(tokens->value);
			tokens->value = new;
		}
		tokens = tokens->next;
	}
}
