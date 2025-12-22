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

static int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

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
    Affiche une ligne au format export : declare -x KEY="VALUE"
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
    Fonction principale :
    1. Convertit la liste en tableau.
    2. Trie le tableau.
    3. Affiche chaque ligne formatée.
    4. Nettoie.
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
