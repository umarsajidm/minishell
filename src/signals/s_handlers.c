#include "minishell.h"

/* Handle SIGINT (Ctrl-C)
 * - Update global signal variable
 * - Move cursor to new line
 * - Clear current readline buffer
 * - Redisplay prompt
 */
void    handle_sigint(int sig)
{
    g_signal = sig;             // store received signal
    write(1, "\n", 1);          // move to new line
    rl_on_new_line();           // notify readline of new line
    rl_replace_line("", 0);     // clear current input
    rl_redisplay();             // redisplay prompt
}

/* Handle SIGQUIT (Ctrl-\)
 * - Shell ignores this signal
 * - Ensure prompt is correctly displayed
 */

void    handle_sigquit(int sig)
{
    (void)sig;
    rl_on_new_line();           // ensure prompt is on new line
    rl_redisplay();             // redisplay prompt
}

void    handle_hd_signal(int sig)
{
    g_signal = sig;
    write(1, "\n", 1);
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