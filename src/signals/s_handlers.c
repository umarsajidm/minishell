/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_handlers.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:17:08 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/20 11:20:20 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Handle SIGINT (Ctrl-C)
 * - Update global signal variable
 * - Move cursor to new line
 * - Clear current readline buffer
 * - Redisplay prompt
 */
void	handle_sigint(int sig)
{
	g_signal = sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_hd_signal(int sig)
{
	g_signal = sig;
	write(STDOUT_FILENO, "\n", 1);
	rl_done = 1;
	close(STDIN_FILENO);
}

void	exec_sigint(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
}

void	exec_sigquit(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
}
