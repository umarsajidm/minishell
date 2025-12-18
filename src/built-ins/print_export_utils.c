#include "minishell.h"

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
