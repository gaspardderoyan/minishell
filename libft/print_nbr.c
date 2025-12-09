/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_nbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 14:57:04 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/28 14:05:10 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Prints a decimal integer to standard output.
 * @param nb   The integer to print.
 * @return     The number of characters printed.
 */
int	print_decimal(int nb)
{
	char	*str;
	int		len;

	str = ft_itoa(nb);
	if (!str)
		return (0);
	len = print_str(str);
	free(str);
	return (len);
}

/**
 * @brief      Prints an unsigned integer to standard output.
 * @param nb   The unsigned integer to print.
 * @return     The number of characters printed.
 */
int	print_uint(unsigned int nb)
{
	char	*numbers;
	int		count;

	numbers = "0123456789";
	count = 0;
	if (nb >= 10)
		count += print_uint(nb / 10);
	count += write(1, &numbers[nb % 10], 1);
	return (count);
}

/**
 * @brief      Prints an unsigned integer in hexadecimal format to 
 * 			   standard output.
 * @param nb   The unsigned integer to print.
 * @param uppercase If non-zero, uses uppercase letters (A-F), otherwise
 * 			   lowercase (a-f).
 * @return     The number of characters printed.
 */
int	print_hexa(unsigned int nb, int uppercase)
{
	char	*hexa;
	int		count;

	count = 0;
	if (uppercase)
		hexa = "0123456789ABCDEF";
	else
		hexa = "0123456789abcdef";
	if (nb >= 16)
		count += print_hexa(nb / 16, uppercase);
	count += write(1, &hexa[nb % 16], 1);
	return (count);
}
