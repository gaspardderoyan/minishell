/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ptr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 17:01:26 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/27 18:03:39 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Recursively prints a memory address in hexadecimal format.
 * @param nb   The address to print.
 * @return     The number of characters printed.
 */
static	int	get_address(unsigned long nb)
{
	char	*hexa;
	int		count;

	hexa = "0123456789abcdef";
	count = 0;
	if (nb >= 16)
		count += get_address(nb / 16);
	count += write(1, &hexa[nb % 16], 1);
	return (count);
}

/**
 * @brief      Prints a pointer address in hexadecimal format to standard output.
 * @param ptr  The pointer to print (if NULL, prints "(nil)").
 * @return     The number of characters printed.
 */
int	print_ptr(void *ptr)
{
	int	count;

	if (!ptr)
		return (write(1, "(nil)", 5));
	count = 0;
	count += print_str("0x");
	count += get_address((unsigned long)ptr);
	return (count);
}
