/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:43:23 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/30 15:27:31 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Entry point of minishell.
 * - Initializes environment, arena, signal handlers
 * - Starts the REPL loop
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;   // shell state (env, exit_code, running)
	t_arena		*arena;  // arena memory allocator

	(void)argc;
	(void)argv;
	(void)envp;

	/* initialize shell state */
	shell.env = NULL;
	shell.exit_code = 0;
	shell.running = true;

	/* initialize arena */
	arena = init_arena(1024);       // 1 KB initial arena
	if (!arena)
	{
		ft_printf("minishell: failed to initialize memory arena\n");
		return (1);
	}

	/* initialize environment (disabled for now) */
	// shell.env = init_env(envp);
	// if (!shell.env)
	// {
	//     ft_printf("minishell: failed to initialize environment\n");
	//     free_arena(&arena);
	//     return (1);
	// }

	setup_signals();                 // setup signal handlers

	repl_loop(&shell, &arena);       // start main REPL loop

	dbg_print_exit_code(shell.exit_code); // debug: print final exit code

	/* cleanup */
	// free_env(shell.env);          // disabled while env functions are not present
	free_arena(&arena);              // free all arena memory

	return (shell.exit_code);        // return shell exit code
}
