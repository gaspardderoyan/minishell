/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dprint_ptr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:00:00 by opencode          #+#    #+#             */
/*   Updated: 2025/12/20 12:00:00 by opencode         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Recursively prints a memory address in hexadecimal format.
 * @param nb   The address to print.
 * @param fd   The file descriptor.
 * @return     The number of characters printed.
 */
static	int	dget_address(unsigned long nb, int fd)
{
	char	*hexa;
	int		count;

	hexa = "0123456789abcdef";
	count = 0;
	if (nb >= 16)
		count += dget_address(nb / 16, fd);
	count += write(fd, &hexa[nb % 16], 1);
	return (count);
}

/**
 * @brief      Prints a pointer address in hexadecimal format to a file descriptor.
 * @param ptr  The pointer to print (if NULL, prints "(nil)").
 * @param fd   The file descriptor.
 * @return     The number of characters printed.
 */
int	dprint_ptr(void *ptr, int fd)
{
	int	count;

	if (!ptr)
		return (write(fd, "(nil)", 5));
	count = 0;
	count += dprint_str("0x", fd);
	count += dget_address((unsigned long)ptr, fd);
	return (count);
}
