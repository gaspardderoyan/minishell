/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isspace.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 22:00:00 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/24 22:00:00 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*
** Checks if the character is a whitespace character.
** Whitespace characters include:
**   - ' '  (space, 32)
**   - '\t' (horizontal tab, 9)
**   - '\n' (newline, 10)
**   - '\v' (vertical tab, 11)
**   - '\f' (form feed, 12)
**   - '\r' (carriage return, 13)
** @param c: The character to check (as int).
** @return: 1 if whitespace, 0 otherwise.
*/
int	ft_isspace(int c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}
