#include "minishell.h"

static int	is_flag(const char *s, int *newline);

int	ft_echo(char **av)
{
	int	i;
	int	newline;

	if (!av[1])
		return ((void)ft_putchar_fd('\n', STDOUT_FILENO), 0);
	i = 1;
	newline = 1;
	while (is_flag(av[i], &newline))
		i++;
	while (av[i])
	{
		ft_putstr_fd(av[i++], STDOUT_FILENO);
		if (av[i])
			ft_putchar_fd(' ', STDOUT_FILENO);
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}

static int	is_flag(const char *s, int *newline)
{
	int	i;

	i = 2;
	if (!s || !*s)
		return (0);
	if (s[0] != '-')
		return (0);
	if (s[1] != 'n')
		return (0);
	while (s[i])
	{
		if (s[i] != 'n')
			return (0);
		i++;
	}
	if (!s[i])
		return (*newline = 0, 1);
	return (0);
}
