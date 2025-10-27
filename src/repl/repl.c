/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:02:03 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/04 16:44:52 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;

	// Initialize environment variables and shell status
	shell.env = init_env(envp);
	shell.exit_code = 0;
	shell.running = true;

	// Setup signal handlers for Ctrl-C and Ctrl-\
	setup_signals();

	// Start the REPL loop
	repl_loop(&shell);

	// Free environment linked list before exiting
	free_env(shell.env);

	return (shell.exit_code);
}
