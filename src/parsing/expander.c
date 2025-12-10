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

// TODO: function to replace 1st $var with it's content
void	expand_token(char *token, char **env)
{
	// TODO: find first $var name in token (it's 'start' and 'end' pointers)
	
	// TODO: find value of $var in **env
	
	// TODO: use ft_str_replace to replace the $var w/ it's value
}

void	expander(t_token *tokens, char **env)
{
	while (tokens)
	{
		if (tokens->type != TOKEN_WORD || tokens->value[0] != '"' ||
			tokens->value[ft_strlen_safe(tokens->value)- 1] != '"' ||
			ft_strchr_safe(tokens->value, '$') == NULL)
			tokens = tokens->next;
		else
			expand_token(tokens->value, env);
	}
}
