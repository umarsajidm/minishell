/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:10:27 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/18 13:10:28 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*skip_whitespace(const char *s);
static int	overflow(long result, int digit, int sign);
static void	init_vars(int **error, int *sign, long *result);

/*
** Converts a string to a long integer (ascii to long).
** Handles signs (+/-), whitespace, and checks for numeric validity.
** Detects overflow/underflow and invalid characters.
** Sets the error flag if conversion fails.
** Returns the converted long value or 0 on error.
*/
long	ft_atol(const char *s, int *error)
{
	long	result;
	int		sign;

	init_vars(&error, &sign, &result);
	s = skip_whitespace(s);
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
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

/*
** Advances the string pointer past any leading whitespace.
** Returns the new pointer position.
*/
static char	*skip_whitespace(const char *s)
{
	while (*s && (*s == ' ' || *s == '\t'))
		s++;
	return ((char *)s);
}

/*
** Checks if adding a digit to the current result would cause an overflow.
** Handles both positive and negative cases (LONG_MAX limits).
** Returns 1 if overflow would occur, 0 otherwise.
*/
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

/*
** Initializes variables used in ft_atol.
** Sets error flag to 0, sign to positive, and result to 0.
*/
static void	init_vars(int **error, int *sign, long *result)
{
	**error = 0;
	*sign = 1;
	*result = 0;
}
