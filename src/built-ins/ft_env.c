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
		ft_putstr_fd(head->key, STDOUT_FILENO);
		if (head->value)
		{
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(head->value, STDOUT_FILENO);
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
		head = head->next;
	}
	return (0);
}
