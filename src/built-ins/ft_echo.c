#include "minishell.h"

int	ft_echo(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	if (argv[1] && ft_strcmp("-n", argv[1]) == 0)
	{
		newline = 0;
		i = 2;
	}
	else
		newline = 1;
	while (argv[i])
	{
		if (printf("%s", argv[i]) < 0)
			return (1);
		i++;
		if (argv[i])
			if (printf(" ") < 0)
				return (1);
	}
	if (newline)
		if (printf("\n") < 0)
			return (1);
	return (0);
}
