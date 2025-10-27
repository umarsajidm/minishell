/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:10:14 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/04 19:56:43 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Global variable to store last received signal */
int	g_signal = 0;

/* Handle Ctrl-C (SIGINT)
   - Set g_signal to signal number
   - Move to a new line
   - Clear current input line in readline
   - Redisplay prompt */
void	handle_sigint(int sig)
{
	(void)sig;
	g_signal = sig;
	write(1, "\n", 1);        /* move to a new line */
	rl_on_new_line();       /* inform readline about new line */
	rl_replace_line("", 0); /* clear current input line */
	rl_redisplay();         /* redisplay prompt */
}

/* Handle Ctrl-\ (SIGQUIT)
   - Does nothing (shell ignores this signal)
   - Redisplay prompt if needed */
void	handle_sigquit(int sig)
{
	(void)sig;
	/* Keep behavior minimal: just ensure prompt is properly redrawn */
	rl_on_new_line();
	rl_redisplay();
}

/* Setup signal handlers for interactive shell */
void	setup_signals(void)
{
	signal(SIGINT, handle_sigint);    /* Ctrl-C */
	signal(SIGQUIT, handle_sigquit);  /* Ctrl-backslash (written as word in comments) */
}
