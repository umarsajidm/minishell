/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:43:23 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/27 16:03:28 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **envp)
{
	t_shell	shell;

	(void)argc;
	(void)argv;

	// Initialize environment and shell status
	shell.env = init_env(envp);
	shell.exit_code = 0;
	shell.running = true;

	// Setup signal handlers
	setup_signals();

	// Start the REPL loop
	repl_loop(&shell);

	// Free environment before exiting
	free_env(shell.env);
	return (shell.exit_code);
}
