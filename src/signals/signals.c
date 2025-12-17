#include "minishell.h"

/* Global variable to store last received signal */
volatile sig_atomic_t	g_signal = 0;


/* Setup signal handlers for interactive shell
 * - SIGINT handled by handle_sigint
 * - SIGQUIT handled by handle_sigquit
 */
void    setup_parent_signals(void)
{
    signal(SIGINT, handle_sigint);    // Ctrl-C
    signal(SIGQUIT, handle_sigquit);  // Ctrl-Slash
}

void    setup_execution_signals(void)
{
    signal(SIGINT, exec_sigint);
    signal(SIGQUIT, exec_sigquit);
}

void    setup_child_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}

void    setup_hd_signals(void)
{
    struct sigaction	sa;

	sa.sa_handler = handle_hd_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}
