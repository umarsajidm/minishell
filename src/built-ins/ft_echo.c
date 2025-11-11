//#include "minishell.h"
#include <stdio.h>
#include <string.h>

static int	is_flag(const char *s)
{
	int	i;

	i = 2;
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
		return (1);
	return (0);
}

int	ft_echo(char **av)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (is_flag(av[i]))
	{
		newline = 0;
		i++;
	}
	while (av[i])
	{
		if (printf("%s", av[i]) < 0)
			return (1);
		i++;
		if (av[i])
			if (printf(" ") < 0)
				return (1);
	}
	if (newline)
		if (printf("\n") < 0)
			return (1);
	return (0);
}

int main(int ac, char **av)
{
	ft_echo(av);
	return (0);
}
