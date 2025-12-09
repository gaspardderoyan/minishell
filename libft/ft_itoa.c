/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 17:06:37 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/06 11:40:40 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Calculates the number of digits needed to represent a number.
 * @param n    The number to analyze.
 * @return     The number of characters needed (including sign if negative).
 */
static size_t	get_alloc_size(long n)
{
	size_t	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n)
	{
		len++;
		n /= 10;
	}
	return (len);
}

/**
 * @brief      Converts an integer to a string.
 * @param n    The integer to convert.
 * @return     A newly allocated string representing the integer, or NULL if the
 *             allocation fails.
 */
char	*ft_itoa(int n)
{
	char	*str;
	long	nb;
	size_t	size;

	nb = n;
	size = get_alloc_size(nb);
	str = (char *)malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	str[size--] = '\0';
	if (nb == 0)
		str[0] = '0';
	if (nb < 0)
	{
		str[0] = '-';
		nb *= -1;
	}
	while (nb > 0)
	{
		str[size--] = '0' + (nb % 10);
		nb /= 10;
	}
	return (str);
}
