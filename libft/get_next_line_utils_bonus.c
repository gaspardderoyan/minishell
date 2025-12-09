/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:27:16 by mgregoir          #+#    #+#             */
/*   Updated: 2025/08/26 14:14:01 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Calculates the length of a string (NULL-safe version).
 * @param s    The string to measure (can be NULL).
 * @return     The length of the string, or 0 if s is NULL.
 */
size_t	ft_strlen_safe(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

/**
 * @brief      Locates the first occurrence of a character in a string
 *             (NULL-safe).
 * @param s    The string to search (can be NULL).
 * @param c    The character to search for.
 * @return     A pointer to the first occurrence, or NULL if not found
 *             or s is NULL.
 */
char	*ft_strchr_safe(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s && *s != (char)c)
		s++;
	if (*s == (char)c)
		return ((char *)s);
	return (NULL);
}

/**
 * @brief      Duplicates a string (NULL-safe version).
 * @param s    The string to duplicate (can be NULL).
 * @return     A pointer to the newly allocated duplicate string,
 *             or NULL on error.
 */
char	*ft_strdup_safe(const char *s)
{
	char	*dup;
	size_t	len;
	size_t	i;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	dup = malloc(sizeof(char) * len + 1);
	if (!dup)
		return (NULL);
	i = 0;
	while (s[i])
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

/**
 * @brief      Joins two strings and frees the first one.
 * @param s1   The first string (will be freed after joining).
 * @param s2   The second string to append.
 * @return     A newly allocated string containing the concatenation,
 *             or NULL on error.
 */
char	*ft_strjoin_gnl(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	str = malloc(sizeof(char) * (ft_strlen_safe(s1) + ft_strlen_safe(s2) + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s1 && s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2 && s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	str[i + j] = '\0';
	free(s1);
	return (str);
}
