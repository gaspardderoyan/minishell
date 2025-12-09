/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:08:48 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/16 14:21:49 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Compares the first 'n' characters of two strings.
 * @param s1   The first null-terminated string to be compared.
 * @param s2   The second null-terminated string to be compared.
 * @param n    The maximum number of characters to compare.
 * @return     An integer. Returns 0 if the strings are equal over the first
 *             'n' characters. Otherwise, returns the difference between the
 *             first differing characters (treated as unsigned char).
 */
int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		if (s1[i] == '\0')
			return (0);
		i++;
	}
	return (0);
}
