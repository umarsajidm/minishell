#include "minishell.h"

/* 
 * Entry point of minishell
 * - Initializes shell state, arena, and environment
 * - Sets up signal handlers
 * - Starts the REPL loop
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;   // shell state: env, exit_code, running
	t_arena		*arena;  // arena memory allocator

	(void)argc;
	(void)argv;

	/* initialize shell state */
	shell.env = NULL;          // env will be initialized below
	shell.exit_code = 0;
	shell.running = true;

	/* initialize arena */
	arena = init_arena(1024);  // 1 KB initial arena
	if (!arena)
	{
		ft_printf("minishell: failed to initialize memory arena\n");
		return (1);
	}

	/* initialize environment from envp */
	shell.env = init_env(envp); // malloc-based env list
	if (!shell.env)
	{
		ft_printf("minishell: failed to initialize environment\n");
		free_arena(&arena);
		return (1);
	}

	/* setup signal handlers */
	setup_signals();

	/* start REPL loop */
	repl_loop(&shell, &arena);

	/* debug: print final exit code */
	dbg_print_exit_code(shell.exit_code);

	/* cleanup */
	free_env(shell.env);       // free malloc'd env list
	free_arena(&arena);        // free all arena memory

	return (shell.exit_code);  // return shell exit code
}
