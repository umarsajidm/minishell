#include "minishell.h"

void	test_builtin(t_cmd *commands, t_shell *shell)
{
	(void)shell;
	if (is_builtin(commands))
		printf("is builtin\n");
	run_builtin(commands, shell);
}