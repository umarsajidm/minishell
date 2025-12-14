#include "minishell.h"

void shell_cleanup(t_shell *shell)
{
    rl_clear_history();
    free_env(shell->env);
    free(shell->arena);
    if (shell->exec && shell->exec->fd)
    {
        close_fd(shell->exec->fd);
        free(shell->exec->fd);
        free(shell->exec);
    }
}