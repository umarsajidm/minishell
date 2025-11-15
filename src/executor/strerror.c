#include "minishell.h"

void	strerrornexit(void)
{
	ft_putstr_fd(strerror(errno), 2);
	ft_putstr_fd("\n", 2);
	exit(EXIT_FAILURE);
}
