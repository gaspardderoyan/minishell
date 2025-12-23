/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dprint_cs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:00:00 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/23 18:30:20 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Prints a single character to a file descriptor.
 * @param c    The character to print.
 * @param fd   The file descriptor.
 * @return     The number of characters printed (always 1).
 */
int	dprint_char(char c, int fd)
{
	return (write(fd, &c, 1));
}

/**
 * @brief      Prints a string to a file descriptor.
 * @param str  The string to print (if NULL, prints "(null)").
 * @param fd   The file descriptor.
 * @return     The number of characters printed.
 */
int	dprint_str(char *str, int fd)
{
	int	len;

	if (!str)
		return (write(fd, "(null)", 6));
	len = ft_strlen(str);
	return (write(fd, str, len));
}
