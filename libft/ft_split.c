/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 13:25:57 by mgregoir          #+#    #+#             */
/*   Updated: 2025/05/21 19:20:54 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief      Counts the number of words in a string separated by a delimiter.
 * @param s    The string to count words from.
 * @param c    The delimiter character.
 * @return     The number of words in the string.
 */
static size_t	count_words(char const *s, char c)
{
	size_t	counter;
	int		in_word;

	if (!s)
		return (0);
	counter = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			counter++;
			in_word = 1;
		}
		else if (*s == c)
			in_word = 0;
		s++;
	}
	return (counter);
}

/**
 * @brief      Frees an array of strings and its content.
 * @param arr  The array to free.
 * @param count The number of elements to free.
 */
static void	free_array(char **arr, size_t count)
{
	size_t	i;

	if (!arr)
		return ;
	i = 0;
	while (i < count)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/**
 * @brief      Fills the array with substrings from the source string.
 * @param arr  The array to fill.
 * @param s    The source string.
 * @param c    The delimiter character.
 * @param word_count The number of words to extract.
 * @return     1 on success, 0 on failure.
 */
static int	fill_array(char **arr, char const *s, char c, size_t word_count)
{
	size_t		i;
	const char	*word_start;

	i = 0;
	while (i < word_count)
	{
		while (s && *s && *s == c)
			s++;
		word_start = s;
		while (s && *s && *s != c)
			s++;
		arr[i] = ft_substr(word_start, 0, (size_t)(s - word_start));
		if (!arr[i])
		{
			free_array(arr, i);
			return (0);
		}
		i++;
	}
	arr[i] = NULL;
	return (1);
}

/**
 * @brief      Splits a string into an array of substrings using a delimiter.
 * @param s    The string to split.
 * @param c    The delimiter character.
 * @return     An array of strings resulting from the split, or NULL if the
 *             allocation fails. The array is terminated by a NULL pointer.
 */
char	**ft_split(char const *s, char c)
{
	char	**array;
	size_t	word_count;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	array = ft_calloc((word_count + 1), sizeof(char *));
	if (!array)
		return (NULL);
	if (!fill_array(array, s, c, word_count))
		return (NULL);
	return (array);
}
