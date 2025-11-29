#include "minishell.h"

void general_cleanup(t_shell *shell, t_arena **arena)
{
    if (shell && shell->env)
        free_env(shell->env);

    if (arena && *arena)
        free_arena(arena); 

    rl_clear_history();
}
