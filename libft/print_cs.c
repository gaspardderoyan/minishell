/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:40:22 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/28 12:42:55 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Prints a single character to standard output.
 * @param c    The character to print.
 * @return     The number of characters printed (always 1).
 */
int	print_char(char c)
{
	return (write(1, &c, 1));
}

/**
 * @brief      Prints a string to standard output.
 * @param str  The string to print (if NULL, prints "(null)").
 * @return     The number of characters printed.
 */
int	print_str(char *str)
{
	int	len;

	if (!str)
		return (write(1, "(null)", 6));
	len = ft_strlen(str);
	return (write(1, str, len));
}
