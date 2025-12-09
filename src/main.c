#include "minishell.h"

static int  init_shell_and_arena(t_shell *shell, t_arena **arena, char **envp)
{
    /* initialize shell state */
    shell->env = NULL;
    shell->exit_code = 0;
    shell->running = true;

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
    shell->exec = ft_calloc(1, sizeof(t_exec));
    if (!shell->exec)
    {
        free_env(shell->env);
        free_arena(arena);
        return (1);
    }
    shell->exec->fd = ft_calloc(1, sizeof(t_fd));
    if (!shell->exec->fd)
    {
        free(shell->exec);
        free_env(shell->env);
        free_arena(arena);
        return (1);
    }
    init_fd(shell->exec->fd);
    return (0);
}

// int main(int argc, char **argv, char **envp)
// {
//     t_shell     shell;   // shell state: env, exit_code, running
//     int         err;     // initialization error

//     (void)argc;
//     (void)argv;

//     /* Safety initialization for shell.fd */

//     err = init_shell_and_arena(&shell, &shell.arena, envp);
//     if (err)
//         return (1);

//     /* setup signal handlers */
//     setup_signals();

//     /* start REPL or non-interactive loop */
//     if (isatty(STDIN_FILENO))
//     {
//         // Interactive Mode: ./minishell
//         repl_loop(&shell, &shell.arena);
//     }
//     else
//     {
//         // Non-Interactive Mode: echo "ls" | ./minishell
//         non_interactive_loop(&shell, &shell.arena);
//     }

//     rl_clear_history();
//     free_env(shell.env);
//     free_arena(&shell.arena);
//     if (shell.exec->fd) 
//         close_fd(shell.exec->fd);
//     free(shell.exec->fd);
//     free(shell.exec);
//     return (shell.exit_code);
// }

int main(int argc, char **argv, char **envp)
{
    t_shell     shell;
    int         err;

    (void)argc;
    (void)argv;

    /* Initialize shell and arena */
    err = init_shell_and_arena(&shell, &shell.arena, envp);
    if (err)
        return (1);

    setup_signals();

    /* * TESTER CHECK:
     * If STDIN is a TTY (keyboard), run REPL.
     * If STDIN is a PIPE (tester script), run non-interactive loop.
     */
    if (isatty(STDIN_FILENO))
    {
        // Interactive Mode: ./minishell
        repl_loop(&shell, &shell.arena);
    }
    else
    {
        // Non-Interactive Mode: echo "ls" | ./minishell
        non_interactive_loop(&shell, &shell.arena);
    }

    /* Cleanup */
    rl_clear_history();
    free_env(shell.env);
    free_arena(&shell.arena);
    if (shell.exec && shell.exec->fd) 
    {
        close_fd(shell.exec->fd);
        free(shell.exec->fd);
        free(shell.exec);
    }
    
    return (shell.exit_code);
}