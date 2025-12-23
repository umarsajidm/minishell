/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:10:20 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/18 13:13:01 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdint.h>

static int	get_ac(char **av);
static int	num_error(char *str);

/*
** Executes the 'exit' built-in command.
** 1. Prints "exit" if run in the main shell (not child).
** 2. If no arguments, returns current exit code and sets flow to exit.
** 3. Checks for "too many arguments".
** 4. Parses argument to integer, handling numeric errors.
** 5. Sets shell exit code (modulo 256) and exit flow.
** Returns the new exit code.
*/
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

/*
** Counts the number of arguments in the null-terminated array.
** Returns the count.
*/
static int	get_ac(char **av)
{
	int	i;

	i = 0;
	while (av[i])
		i++;
	return (i);
}

/*
** Handles numeric argument errors for exit.
** Prints specific error message for non-numeric input.
** Returns 2 (standard exit code for syntax errors in exit).
*/
static int	num_error(char *str)
{
	ft_putstr_fd("minishell: exit:", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(" numeric argument required\n", 2);
	return (2);
}
