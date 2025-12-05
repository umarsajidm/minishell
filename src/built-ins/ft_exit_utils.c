#include "minishell.h"

static char	*skip_whitespace(const char *s);
static int	process_sign(const char **s);
static int	overflow(long result, int digit, int sign);

long	ft_atol(const char *s, int *error)
{
	long	result;
	int		sign;

	*error = 0;
	sign = 0;
	result = 0;
	s = skip_whitespace(s);
	sign = process_sign(&s);
	if (!ft_isdigit(*s))
		return (*error = 1, 0);
	while (ft_isdigit(*s))
	{
		if (overflow(result, ((*s) - '0'), sign))
			return (*error = 1, 0);
		result = result * 10 + (*s - '0');
		s++;
	}
	s = skip_whitespace(s);
	if (*s != '\0')
		return (*error = 1, 0);
	return (result * sign);
}

static char	*skip_whitespace(const char *s)
{
	while (*s && (*s == ' ' || *s == '\t'))
		s++;
	return ((char *)s);
}

static int	process_sign(const char **s)
{
	int	sign;

	sign = 1;
	if (**s == '+' || **s == '-')
	{
		if (**s == '-')
			sign = -1;
		(*s)++;
	}
	return (sign);
}

static int	overflow(long result, int digit, int sign)
{
	if (sign == 1)
	{
		if (result > (LONG_MAX - digit) / 10)
			return (1);
	}
	else
	{
		if (result > LONG_MAX / 10)
			return (1);
		if (result == LONG_MAX && digit > (LONG_MAX % 10) + 1)
			return (1);
	}
	return (0);
}