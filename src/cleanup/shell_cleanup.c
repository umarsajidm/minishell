#include "minishell.h"

void execution_cleanup(t_shell *shell, char **arr)
{
    if (shell && shell->env)
        free_env(shell->env);

    if (shell->arena)
		arena_clear(&shell->arena);

	// close_fd(shell->fd);

	if (shell->exec->fd != NULL)
		free(shell->exec->fd);

	freearray(arr);

    // rl_clear_history();
}
