#include "minishell.h"

void	test_builtin(t_cmd *commands, t_shell *shell)
{
	(void)shell;
	if (is_builtin(commands))
		printf("built-in exit status: %d\n", run_builtin(commands, shell));
}