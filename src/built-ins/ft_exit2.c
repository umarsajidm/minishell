#include "minishell.h"

static void    clean_all(t_env *head, t_arena **arena, t_shell *shell);
static void    run_exit(t_shell *shell);

int    ft_exit2(char **av, t_shell *shell)
{
    ft_putstr_fd("exit\n", 1);
    if (!av)
	{
        run_exit(shell);
	}
	return (0);
}

static void    clean_all(t_env *head, t_arena **arena, t_shell *shell)
{
    rl_clear_history();
    free_env(head);
    free_arena(arena);
    if (shell->exec->fd)
    {
        free(shell->exec->fd);
        shell->exec->fd = NULL;
    }
    if (shell->exec)
    {
        free(shell->exec);
        shell->exec = NULL;
    }
}

static void    run_exit(t_shell *shell)
{
    long    exit_code;

    exit_code = shell->exit_code;
    clean_all(shell->env, &shell->arena, shell);
    exit(exit_code & 0xFF);
}
