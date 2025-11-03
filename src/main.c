<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:43:23 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/30 13:54:02 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char **copy_envp(char **envp)
{
	char **env;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	env = malloc(sizeof(char *) *(i + 1));
	if (!env)
		return NULL;
	i = 0;
	while(envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}

=======
#include "minishell.h"

/* 
 * Entry point of minishell.
 * - Initializes environment, arena, signal handlers
 * - Starts the REPL loop
 */
>>>>>>> main
int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;   // shell state (env, exit_code, running)
	t_arena		*arena;  // arena memory allocator

	(void)argc;
	(void)argv;
	char **env;

	env = copy_envp(envp);

<<<<<<< HEAD
	// Initialize environment and shell status
  //shell.env = init_env(envp);
=======
	/* initialize shell state */
	shell.env = NULL;
>>>>>>> main
	shell.exit_code = 0;
	shell.running = true;

	/* initialize arena */
	arena = init_arena(1024);       // 1 KB initial arena
	if (!arena)
	{
		ft_printf("minishell: failed to initialize memory arena\n");
		return (1);
	}

<<<<<<< HEAD
	// Start the REPL loop
	repl_loop(&shell, env);

	// Free environment before exiting
	//free_env(shell.env);
	free(env);
	return (shell.exit_code);
=======
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
>>>>>>> main
}
