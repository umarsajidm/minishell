#include "minishell.h"

int	ft_env(t_env *head)
{
	while (head)
	{
		printf("%s=%s\n", head->key, head->value);
		head = head->next;
	}
	return (0);
}
