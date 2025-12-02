#include "minishell.h"

void	test_builtin(t_cmd *commands, t_shell *shell, t_arena **arena)
{
	(void)shell;
	if (is_builtin(commands))
		shell->exit_code = run_builtin(commands, shell, arena);
}