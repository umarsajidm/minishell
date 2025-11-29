#include "minishell.h"

static int	is_flag(const char *s, int *newline);

int	ft_echo(char **av)
{
	int	i;
	int	newline;

	if (!av[1])
		return ((void)printf("\n"), 0);
	i = 1;
	newline = 1;
	while (is_flag(av[i], &newline))
		i++;
	while (av[i])
	{
		printf("%s", av[i++]);
		if (av[i])
			printf(" ");
	}
	if (newline)
		printf("\n");
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
