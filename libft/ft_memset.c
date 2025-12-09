/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:50:24 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/05 17:51:19 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Fills the first 'n' bytes of a memory area with a constant byte.
 * @param s    A pointer to the memory area to fill.
 * @param c    The byte value to fill with (interpreted as an unsigned char).
 * @param n    The number of bytes to fill.
 * @return     A pointer to the filled memory area ('s').
 */
void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*temp;
	size_t			i;

	temp = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		temp[i] = (unsigned char)c;
		i++;
	}
	return (s);
}
