#include "minishell.h"

void execution_cleanup(t_shell *shell)
{
    if (shell->env)
        free_env(shell->env);

    if (shell->arena)
        free_arena(&shell->arena);

    if (shell->exec->fd)
        free(shell->exec->fd);

    if (shell->exec)
        free(shell->exec);
}
