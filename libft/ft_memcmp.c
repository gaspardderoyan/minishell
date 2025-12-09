/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:32:28 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/14 11:05:35 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Compares the first 'n' bytes of two memory blocks.
 * @param s1   Pointer to the first block of memory.
 * @param s2   Pointer to the second block of memory.
 * @param n    The number of bytes to be compared.
 * @return     An integer. Returns 0 if the blocks are equal, or the difference
 *             between the first two differing bytes (treated as unsigned char).
 */
int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*p1;
	unsigned char	*p2;
	size_t			i;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (p1[i] != p2[i])
			return (p1[i] - p2[i]);
		i++;
	}
	return (0);
}
