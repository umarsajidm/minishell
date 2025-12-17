#include "minishell.h"
#include <stdint.h>

static int	get_ac(char **av);
static int	num_error(char *str);

int	ft_exit(char **av, t_shell *shell, t_arena **arena, bool is_child_process)
{
	long	exit_code;
	int		error;
	long	result;

	(void)arena;
	if (!is_child_process)
		shell->should_print_exit_message = true;
	if (!av[1])
	{
		shell->exit_flow = FLOW_EXIT;
		return (shell->exit_code);
	}
	else if (get_ac(av) > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	result = ft_atol(av[1], &error);
	if (error)
		exit_code = num_error(av[1]);
	else
		exit_code = result;
	shell->exit_code = exit_code & 0xFF;
	shell->exit_flow = FLOW_EXIT;
	return (shell->exit_code);
}

static int	get_ac(char **av)
{
	int	i;

	i = 0;
	while (av[i])
		i++;
	return (i);
}

static int	num_error(char *str)
{
	ft_putstr_fd("minishell: exit:", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(" numeric argument required\n", 2);
	return (2);
}

