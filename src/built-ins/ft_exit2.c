/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:10:52 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/20 13:36:35 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_exit(t_shell *shell);

/*
** Prints "exit" and terminates the shell.
** This variant is likely used when exit is called without arguments
** or in specific contexts (like Ctrl+D).
*/
int	ft_exit2(char **av, t_shell *shell)
{
	ft_putstr_fd("exit\n", 1);
	if (!av)
	{
		run_exit(shell);
	}
	return (0);
}

/*
** Performs final shell cleanup and exits the process.
** Uses the stored exit code from the shell structure.
*/
static void	run_exit(t_shell *shell)
{
	long	exit_code;

	exit_code = shell->exit_code;
	shell_cleanup(shell);
	exit(exit_code & 0xFF);
}
