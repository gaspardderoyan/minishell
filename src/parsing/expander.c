#include "libft.h"
#include "minishell.h"

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
