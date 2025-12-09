/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:57:07 by mgregoir          #+#    #+#             */
/*   Updated: 2025/04/30 15:10:44 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Scans a memory area for a character.
 * @param s    A pointer to the memory area to scan.
 * @param c    The character to search for (interpreted as an unsigned char).
 * @param n    The number of bytes to scan.
 * @return     A pointer to the matching byte, or NULL if the character is not
 *             found within the first 'n' bytes.
 */
void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*tmp;

	tmp = (unsigned char *)s;
	while (n--)
	{
		if (*tmp == (unsigned char)c)
			return ((void *)tmp);
		tmp++;
	}
	return (NULL);
}
