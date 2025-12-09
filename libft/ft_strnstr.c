/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 12:13:56 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/02 12:59:31 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Locates the first occurrence of a substring in a string,
 *             searching at most 'len' characters.
 * @param big    The string in which to search.
 * @param little The null-terminated substring to search for.
 * @param len    The maximum number of characters to search in 'big'.
 * @return     A pointer to the beginning of the located substring, or NULL
 *             if the substring is not found. If 'little' is an empty string,
 *             'big' is returned.
 */
char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	l_len;

	if (*little == '\0')
		return ((char *)big);
	l_len = ft_strlen(little);
	i = 0;
	while (big[i] != '\0' && i < len)
	{
		if (i + l_len <= len && ft_strncmp(big + i, little, l_len) == 0)
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}
