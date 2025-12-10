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
		ft_putstr_fd(head->key, STDERR_FILENO);
		if (head->value)
		{
			ft_putchar_fd('=', STDERR_FILENO);
			ft_putstr_fd(head->value, STDERR_FILENO);
		}
		ft_putchar_fd('\n', STDERR_FILENO);
		head = head->next;
	}
	return (0);
}
