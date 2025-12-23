/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:09:38 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/18 13:09:40 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Counts the number of nodes in the environment linked list.
** Returns the count.
*/
int	get_env_length(t_env *head)
{
	int	len;

	len = 0;
	while (head)
	{
		len++;
		head = head->next;
	}
	return (len);
}

/*
** Allocates an array of pointers to environment nodes.
** Creates a linear array from the linked list to facilitate sorting.
** Returns the allocated array or NULL on failure.
*/
t_env	**alloc_assign(int len, t_env *head)
{
	t_env	**strings;
	int		i;

	i = 0;
	strings = (t_env **)malloc((len + 1) * sizeof(t_env *));
	if (!strings)
		return (NULL);
	strings[len] = NULL;
	while (head)
	{
		strings[i] = head;
		i++;
		head = head->next;
	}
	return (strings);
}

/*
** Sorts an array of environment variable pointers alphabetically by key.
** Uses the bubble sort algorithm.
*/
void	sort_env(t_env ***arr)
{
	t_env	*temp;
	int		swaps;
	int		i;

	swaps = 1;
	while (swaps)
	{
		swaps = 0;
		i = 0;
		while ((*arr)[i + 1])
		{
			if (ft_strcmp((*arr)[i]->key, (*arr)[i + 1]->key) > 0)
			{
				temp = (*arr)[i];
				(*arr)[i] = (*arr)[i + 1];
				(*arr)[i + 1] = temp;
				swaps++;
			}
			i++;
		}
	}
}

/*
** Prints environment variables in the format required by 'export' (no args).
** Format: declare -x KEY="VALUE"
** Handles empty values and unset values appropriately.
*/
void	print_export(t_env **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		if (arr[i]->value)
		{
			if (!*(arr[i]->value))
				printf("declare -x %s=\"\"\n", arr[i]->key);
			else
				printf("declare -x %s=%s\n", arr[i]->key, arr[i]->value);
		}
		else
			printf("declare -x %s\n", arr[i]->key);
		i++;
	}
}
