/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 15:46:59 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/28 12:42:33 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Dispatches the appropriate print function based on the format 
 *             specifier.
 * @param ap   The variable argument list.
 * @param specifier The format specifier character.
 * @return     The number of characters printed.
 */
static int	dispatch_format(va_list ap, char specifier)
{
	int	count;

	count = 0;
	if (specifier == 'c')
		count = print_char((char)va_arg(ap, int));
	else if (specifier == 's')
		count = print_str(va_arg(ap, char *));
	else if (specifier == 'p')
		count = print_ptr(va_arg(ap, void *));
	else if (specifier == 'd' || specifier == 'i')
		count = print_decimal(va_arg(ap, int));
	else if (specifier == 'u')
		count = print_uint(va_arg(ap, unsigned int));
	else if (specifier == 'x')
		count = print_hexa(va_arg(ap, unsigned int), 0);
	else if (specifier == 'X')
		count = print_hexa(va_arg(ap, unsigned int), 1);
	else if (specifier == '%')
		count = print_char(specifier);
	return (count);
}

/**
 * @brief      A simplified implementation of printf.
 * @param format The format string containing text and format specifiers.
 * @param ...  Variable arguments to be formatted and printed.
 * @return     The total number of characters printed, or -1 on error.
 */
int	ft_printf(const char *format, ...)
{
	va_list	ap;
	int		i;
	int		count;

	if (!format)
		return (-1);
	va_start(ap, format);
	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] == '%' && format[i + 1])
		{
			count += dispatch_format(ap, format[i + 1]);
			i++;
		}
		else
			count += print_char(format[i]);
		i++;
	}
	va_end(ap);
	return (count);
}
