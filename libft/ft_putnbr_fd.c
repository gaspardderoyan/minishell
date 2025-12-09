/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 18:18:51 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/02 17:39:28 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief     Outputs the integer 'n' to the given file descriptor.
 * @param n   The integer to output.
 * @param fd  The file descriptor on which to write.
 */
void	ft_putnbr_fd(int n, int fd)
{
	long	l;
	char	c;

	l = n;
	if (l < 0)
	{
		ft_putchar_fd('-', fd);
		l = -l;
	}
	if (l >= 10)
		ft_putnbr_fd(l / 10, fd);
	c = (l % 10) + '0';
	ft_putchar_fd(c, fd);
}
