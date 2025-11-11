#include "minishell.h"

/* 
 * Entry point of minishell
 * - Initializes shell state, arena, and environment
 * - Sets up signal handlers
 * - Starts the REPL loop
 */
int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;   // shell state: env, exit_code, run
	t_arena		*arena;
	(void)argc;
	(void)argv;

	ft_bzero(&shell, sizeof(t_shell));
	
	init_shell(&shell, envp, &arena);
	/* setup signal handlers */
	setup_signals();

	/* start REPL loop */
	repl_loop(&shell, &arena);

	/* debug: print final exit code */
	// dbg_print_exit_code(shell.exit_code);
	/* cleanup */
	free_env(shell.env, NULL);       // free malloc'd env list
	free_arena(&arena);        // free all arena memory
	rl_clear_history();
	return (shell.exit_code);  // return shell exit code
}
