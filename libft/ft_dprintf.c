/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 12:00:00 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/23 18:30:49 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Dispatches the appropriate print function based on the format 
 *             specifier.
 * @param ap   The variable argument list.
 * @param specifier The format specifier character.
 * @param fd   The file descriptor.
 * @return     The number of characters printed.
 */
static int	ddispatch_format(va_list ap, char specifier, int fd)
{
	int	count;

	count = 0;
	if (specifier == 'c')
		count = dprint_char((char)va_arg(ap, int), fd);
	else if (specifier == 's')
		count = dprint_str(va_arg(ap, char *), fd);
	else if (specifier == 'p')
		count = dprint_ptr(va_arg(ap, void *), fd);
	else if (specifier == 'd' || specifier == 'i')
		count = dprint_decimal(va_arg(ap, int), fd);
	else if (specifier == 'u')
		count = dprint_uint(va_arg(ap, unsigned int), fd);
	else if (specifier == 'x')
		count = dprint_hexa(va_arg(ap, unsigned int), 0, fd);
	else if (specifier == 'X')
		count = dprint_hexa(va_arg(ap, unsigned int), 1, fd);
	else if (specifier == '%')
		count = dprint_char(specifier, fd);
	return (count);
}

/**
 * @brief      A simplified implementation of dprintf.
 * @param fd   The file descriptor to print to.
 * @param format The format string containing text and format specifiers.
 * @param ...  Variable arguments to be formatted and printed.
 * @return     The total number of characters printed, or -1 on error.
 */
int	ft_dprintf(int fd, const char *format, ...)
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
			count += ddispatch_format(ap, format[i + 1], fd);
			i++;
		}
		else
			count += dprint_char(format[i], fd);
		i++;
	}
	va_end(ap);
	return (count);
}
