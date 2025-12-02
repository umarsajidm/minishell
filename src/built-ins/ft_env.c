#include "minishell.h"

int	ft_env(t_env *head)
{
	while (head)
	{
		if (!head->value)
		{
			head = head->next;
			continue;
		}
		if (head->value)
			printf("%s=%s\n", head->key, head->value);
		else
			printf("%s\n", head->key);
		head = head->next;
	}
	return (0);
}
