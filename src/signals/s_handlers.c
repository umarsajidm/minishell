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

/* Handle SIGQUIT (Ctrl-\)
 * - Shell ignores this signal
 * - Ensure prompt is correctly displayed
 */

void	handle_sigquit(int sig)
{
	(void)sig;
	rl_on_new_line();
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
