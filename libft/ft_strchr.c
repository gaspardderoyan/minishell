/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:37:27 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/12 13:03:07 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Locates the first occurrence of a character in a string.
 * @param s    The null-terminated string to be searched.
 * @param c    The character to be located.
 * @return     A pointer to the first occurrence of the character, or NULL if
 *             the character is not found. The terminating null byte is
 *             considered part of the string.
 */
char	*ft_strchr(const char *s, int c)
{
	while (*s && *s != (char)c)
		s++;
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}
