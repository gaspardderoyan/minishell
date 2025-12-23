/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 15:55:41 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/22 18:03:41 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Compares two strings lexicographically.
** @param s1: First string.
** @param s2: Second string.
** @return: Difference between first differing characters, or 0 if equal.
*/
static int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/*
** Sorts an array of strings alphabetically using bubble sort.
** @param tab: The array to sort.
** @param size: Number of elements in the array.
*/
static void	sort_env_array(char **tab, int size)
{
	int		i;
	int		j;
	char	*temp;
	int		swapped;

	i = 0;
	while (i < size - 1)
	{
		swapped = 0;
		j = 0;
		while (j < size - 1 - i)
		{
			if (ft_strcmp(tab[j], tab[j + 1]) > 0)
			{
				temp = tab[j];
				tab[j] = tab[j + 1];
				tab[j + 1] = temp;
				swapped = 1;
			}
			j++;
		}
		if (!swapped)
			break ;
		i++;
	}
}

/*
** Prints an environment variable in export format: declare -x KEY="VALUE".
** Variables without '=' are printed without quotes.
** @param str: The environment string (e.g., "PATH=/usr/bin" or "MYVAR").
*/
static void	print_export_line(char *str)
{
	char	*equal_sign;

	ft_putstr_fd("declare -x ", STDOUT_FILENO);
	equal_sign = ft_strchr(str, '=');
	if (equal_sign)
	{
		write(STDOUT_FILENO, str, equal_sign - str);
		write(STDOUT_FILENO, "=\"", 2);
		ft_putstr_fd(equal_sign + 1, STDOUT_FILENO);
		write(STDOUT_FILENO, "\"", 1);
	}
	else
		ft_putstr_fd(str, STDOUT_FILENO);
	printf("\n");
}

/*
** Prints all environment variables in sorted export format.
** Excludes the special '_' variable.
** @param data: Global data structure containing env_list.
** @return: 0 on success, 1 on malloc error.
*/
int	print_sorted_env(t_data *data)
{
	char	**tab;
	int		count;
	int		i;

	tab = env_list_to_array(data->env_list);
	if (!tab)
		return (1);
	count = 0;
	while (tab[count])
		count++;
	sort_env_array(tab, count);
	i = 0;
	while (i < count)
	{
		if (ft_strncmp(tab[i], "_=", 2) != 0)
			print_export_line(tab[i]);
		i++;
	}
	ft_free_array(tab);
	return (0);
}
