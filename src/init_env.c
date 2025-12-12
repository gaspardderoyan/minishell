/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgregoir <mgregoir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:12:46 by mgregoir          #+#    #+#             */
/*   Updated: 2025/12/09 19:53:51 by mgregoir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_list	*init_env_list(char **env)
{
	t_list	*env_list;
	t_list	*new_node;
	char	*content;
	int		i;

	env_list = NULL;
	i = 0;
	if (!env)
		return (NULL);
	while (env[i])
	{
		content = ft_strdup(env[i]);
		if (!content)
			return (NULL);
		new_node = ft_lstnew(content);
		if (!new_node)
		{
			free(content);
			return (NULL);
		}
		ft_lstadd_back(&env_list, new_node);
		i++;
	}
	return (env_list);
}

static char	**env_list_to_array(t_list *env_list)
{
	char	**env_array;
	int		count;
	int		i;

	count = ft_lstsize(env_list);
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	while (env_list)
	{
		if (env_list->content)
		{
			env_array[i] = ft_strdup((char *)env_list->content);
			if (!env_array[i])
			{
				ft_free_array(env_array);
				return (NULL);
			}
		}
		env_list = env_list->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

int main(int ac, char **av, char **env)
{
    (void)ac;
    (void)av;

    t_list *env_list = init_env_list(env);
    if (!env_list)
    {
        printf("Erreur: La liste est vide ou l'allocation a échoué.\n");
        return (1);
    }
    t_list *temp = env_list;
    int count = 0;
    while (temp)
    {
        if (count < 10)
            printf("[LST] Node %d : %s\n", count, (char *)temp->content);
        temp = temp->next;
        count++;
    }
    printf("... (Total %d variables dans la liste)\n", count);

    char **new_env = env_list_to_array(env_list);
    if (!new_env)
    {
        printf("Erreur: La conversion en array a échoué.\n");
        return (1);
    }
    int i = 0;
    while (new_env[i])
    {
        if (i < 10)
            printf("[ARR] Index %d : %s\n", i, new_env[i]);
        i++;
    }
    printf("... (Total %d variables dans l'array)\n", i);
    if (new_env[i] == NULL)
        printf("SUCCÈS : Le tableau se termine bien par NULL.\n");
    else
        printf("ERREUR : Le tableau ne finit pas par NULL.\n");
    i = 0;
    ft_free_array(new_env);
    ft_lstclear(&env_list, free);    
    return (0);
}
