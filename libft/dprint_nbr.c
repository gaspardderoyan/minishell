/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dprint_nbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:00:00 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/23 18:30:31 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Prints a decimal integer to a file descriptor.
 * @param nb   The integer to print.
 * @param fd   The file descriptor.
 * @return     The number of characters printed.
 */
int	dprint_decimal(int nb, int fd)
{
	char	*str;
	int		len;

	str = ft_itoa(nb);
	if (!str)
		return (0);
	len = dprint_str(str, fd);
	free(str);
	return (len);
}

/**
 * @brief      Prints an unsigned integer to a file descriptor.
 * @param nb   The unsigned integer to print.
 * @param fd   The file descriptor.
 * @return     The number of characters printed.
 */
int	dprint_uint(unsigned int nb, int fd)
{
	char	*numbers;
	int		count;

	numbers = "0123456789";
	count = 0;
	if (nb >= 10)
		count += dprint_uint(nb / 10, fd);
	count += write(fd, &numbers[nb % 10], 1);
	return (count);
}

/**
 * @brief      Prints an unsigned integer in hexadecimal format to 
 * 			   a file descriptor.
 * @param nb   The unsigned integer to print.
 * @param uppercase If non-zero, uses uppercase letters (A-F), otherwise
 * 			   lowercase (a-f).
 * @param fd   The file descriptor.
 * @return     The number of characters printed.
 */
int	dprint_hexa(unsigned int nb, int uppercase, int fd)
{
	char	*hexa;
	int		count;

	count = 0;
	if (uppercase)
		hexa = "0123456789ABCDEF";
	else
		hexa = "0123456789abcdef";
	if (nb >= 16)
		count += dprint_hexa(nb / 16, uppercase, fd);
	count += write(fd, &hexa[nb % 16], 1);
	return (count);
}
