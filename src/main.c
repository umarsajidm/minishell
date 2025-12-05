#include "minishell.h"

static int	init_shell_and_arena(t_shell *shell, t_arena **arena,
	char **envp)
{
	/* initialize shell state */
	shell->env = NULL;
	shell->exit_code = 0;
	shell->running = true;
	shell->fd = NULL;
	
	/* initialize arena */
	*arena = init_arena(1024);
	if (!*arena)
	{
		ft_printf("minishell: failed to initialize memory arena\n");
		return (1);
	}
	/* initialize environment from envp */
	shell->env = init_env(envp);
	if (!shell->env)
	{
		ft_printf("minishell: failed to initialize environment\n");
		free_arena(arena);
		return (1);
	}
	shell->fd = malloc(sizeof(t_fd));
	if (!shell->fd)
	{
		// FIX: Must free ALL previously allocated memory on failure path
		ft_putstr_fd("initialization of fd structure failed", 2);
		free_arena(arena);
		free_env(shell->env); 
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell		shell;   // shell state: env, exit_code, running
	int			err;    // initialization error

	(void)argc;
	(void)argv;
    
    // Safety initialization for shell.fd, in case init_shell_and_arena fails early
    shell.fd = NULL;

	err = init_shell_and_arena(&shell, &shell.arena, envp);
	if (err)
		return (1);

	/* setup signal handlers */
	setup_signals();

	/* start REPL loop */
	repl_loop(&shell, &shell.arena);

	/* cleanup */
	// Order doesn't matter much for main cleanup, but all must be freed
	if (shell.fd)
        free(shell.fd); // FIXES Loss Record 14
	rl_clear_history();
	free_env(shell.env);
	free_arena(&shell.arena);

	return (shell.exit_code);
}