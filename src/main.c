#include "minishell.h"


char **copy_envp(char **envp)
{
	int i = 0;
	char **env;
	
	while (envp[i])
		i++;
	env = malloc(sizeof(i + 1));
	i = 0;
	while (envp[i])
	{
		env[i] = ft_strdup(envp[i]);
		i++;
	}
	env[i] = NULL;
	return (env);
}

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


	/* initialize arena */
	arena = init_arena(1024);       // 1 KB initial arena
	if (!arena)
	{
		ft_printf("minishell: failed to initialize memory arena\n");
		return (1);
	}


	setup_signals();                 // setup signal handlers

	repl_loop(&shell, &arena, envp);       // start main REPL loop

	dbg_print_exit_code(shell.exit_code); // debug: print final exit code

	/* cleanup */
	free_arena(&arena);              // free all arena memory
	return (shell.exit_code);        // return shell exit code
}
