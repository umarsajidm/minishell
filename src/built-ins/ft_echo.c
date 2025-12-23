/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:10:33 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/18 13:10:34 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_flag(const char *s, int *newline);

/*
** Executes the 'echo' built-in command.
** 1. Checks for arguments; prints newline if none.
** 2. Parses '-n' flags to suppress the trailing newline.
** 3. Prints remaining arguments separated by spaces.
** 4. Prints a final newline unless suppressed.
** Returns 0.
*/
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

/*
** Checks if a string is a valid '-n' flag (e.g., "-n", "-nn", "-nnn").
** If valid, sets the newline pointer to 0 (false) and returns 1 (true).
** Otherwise, returns 0 (false).
*/
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
