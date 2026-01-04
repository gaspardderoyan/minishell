/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gderoyan <gderoyan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/16 17:00:00 by gderoyan          #+#    #+#             */
/*   Updated: 2025/12/16 16:51:32 by gaspardderoya    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "minishell.h"

/*
** Calculates the number of elements in a NULL-terminated string array.
** @param arr: The string array.
** @return: The number of elements in the array, or 0 if NULL.
*/
static int	ft_arrlen(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return (0);
	while (arr[i])
		i++;
	return (i);
}

/*
** Appends a new string to a NULL-terminated string array.
** Allocates a new array and frees the old one.
** @param arr: The original string array (can be NULL).
** @param str: The string to append.
** @return: The new array with the appended string, or NULL on malloc failure.
*/
char	**ft_append_str(char **arr, char *str)
{
	char	**new_arr;
	int		len;
	int		i;

	if (!str)
		return (arr);
	len = ft_arrlen(arr);
	new_arr = malloc(sizeof(char *) * (len + 2));
	if (!new_arr)
		return (NULL);
	i = -1;
	while (++i < len)
		new_arr[i] = arr[i];
	new_arr[i] = str;
	new_arr[i + 1] = NULL;
	if (arr)
		free(arr);
	return (new_arr);
}
