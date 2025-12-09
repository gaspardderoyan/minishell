/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:39:11 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/14 11:04:29 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Safely appends a source string to a destination buffer.
 * @param dst  The destination buffer.
 * @param src  The null-terminated source string to append.
 * @param size The full size of the destination buffer.
 * @return     The total length of the string that would have been created
 *             (initial length of dst + length of src).
 */
size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d_len;
	size_t	s_len;

	d_len = 0;
	while (d_len < size && dst[d_len])
		d_len++;
	s_len = ft_strlen(src);
	if (d_len == size)
		return (size + s_len);
	if (s_len < size - d_len)
		ft_memcpy(dst + d_len, src, s_len + 1);
	else
	{
		ft_memcpy(dst + d_len, src, size - d_len - 1);
		dst[size - 1] = '\0';
	}
	return (d_len + s_len);
}
