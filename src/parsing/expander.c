#include "libft.h"
#include "minishell.h"
#include "lexer.h"

char *ft_str_replace(char *orig, char *rep, char *start, char *end)
{
	char	*res;
	int		len;
	int		offset;

	len = (ft_strlen_safe(orig) - (end - start) + ft_strlen_safe(rep));
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
		return (NULL);
	ft_memcpy(res, orig, start - orig);
	offset = start - orig;
	ft_memcpy(res + offset, rep, ft_strlen_safe(rep));
	offset += ft_strlen_safe(rep);
	ft_memcpy(res + offset, end, ft_strlen_safe(end));
	res[len] = 0;
	return (res);
}

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

// TODO: function to replace 1st $var with it's content
char	*expand_token(char *token, char **env)
{
	char	*var_name;
	char *dollar;
	char **splits;
	char	*new;
	int i;

	dollar = ft_strchr_safe(token, '$');
	var_name = ft_substr(dollar + 1, 0, get_var_len(dollar + 1));
	if (!var_name) // TODO: fix/improve error/memory handling
		return (NULL);
	i = 0;
	while (env[i])
	{
		// TODO: replace by char *ft_getenv(char *var_name, char **env), using strncmp
		splits = ft_split(env[i], '=');
		if (strcmp(splits[0], var_name) == 0) // TODO: replace by ft_
		{
			new = ft_str_replace(token, splits[1], dollar, dollar + get_var_len(dollar + 1) + 1);
			return (new);
		}
		i++;
	}
	new = ft_str_replace(token, "", dollar, dollar + get_var_len(dollar + 1) + 1);
	return (new);

}

void	expander(t_token *tokens, char **env)
{
	char	*new;

	while (tokens)
	{
		if (tokens->type == TOKEN_WORD && tokens->value[0] == '"' &&
			tokens->value[ft_strlen_safe(tokens->value)- 1] == '"' &&
			ft_strchr_safe(tokens->value, '$') != NULL)
		{
			new = expand_token(tokens->value, env);
			free(tokens->value);
			tokens->value = new;
		}
		else
			tokens = tokens->next;
	}
}
