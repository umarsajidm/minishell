#include "minishell.h"
#include <stdint.h>

static void	run_exit(t_shell *shell);
static int	get_ac(char **av);
static int	num_error(char *str);

int	ft_exit(char **av, t_shell *shell)
{
	long	exit_code;
	int		error;
	long	result;

	ft_putstr_fd("exit\n", 2);
	if (!av[1])
		run_exit(shell);
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
	//TODO: clean_up_shell(shell);
	exit(exit_code & 0xFF);
}

static void	run_exit(t_shell *shell)
{
	long	exit_code;

	exit_code = shell->exit_code;
	exit(exit_code & 0xFF);
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
