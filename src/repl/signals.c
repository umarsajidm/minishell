/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:10:14 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/28 19:08:43 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Global variable to store last received signal */
int g_signal = 0;

/* Handle SIGINT (Ctrl-C)
 * - Update global signal variable
 * - Move cursor to new line
 * - Clear current readline buffer
 * - Redisplay prompt
 */
void handle_sigint(int sig)
{
    (void)sig;
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
void handle_sigquit(int sig)
{
    (void)sig;
    rl_on_new_line();           // ensure prompt is on new line
    rl_redisplay();             // redisplay prompt
}

/* Setup signal handlers for interactive shell
 * - SIGINT handled by handle_sigint
 * - SIGQUIT handled by handle_sigquit
 */
void setup_signals(void)
{
    signal(SIGINT, handle_sigint);    // Ctrl-C
    signal(SIGQUIT, handle_sigquit);  // Ctrl-Slash
}
