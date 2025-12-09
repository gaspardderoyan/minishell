/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:24:04 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/02 11:30:08 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Locates the last occurrence of a character in a string.
 * @param s    The null-terminated string to be searched.
 * @param c    The character to be located.
 * @return     A pointer to the last occurrence of the character, or NULL if
 *             the character is not found. The terminating null byte is
 *             considered part of the string.
 */
char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = ft_strlen(s);
	while (i >= 0)
	{
		if (s[i] == (char)c)
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}
