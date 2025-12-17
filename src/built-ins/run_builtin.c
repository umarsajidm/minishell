#include "minishell.h"
#include <stdbool.h>

int	run_builtin(t_cmd *cmds, t_shell *shell, bool is_child_process)
{
	char	*command;
	int		ret;

	ret = 0;
	command = cmds->argv[0];
	// printf("in run_builtin()\n");
	if (ft_strcmp(command, "echo") == 0)
		ret = ft_echo(cmds->argv);
	else if (ft_strcmp(command, "cd") == 0)
		ret = ft_cd(cmds, shell);
	else if (ft_strcmp(command, "pwd") == 0)
		ret = ft_pwd();
	else if (ft_strcmp(command, "export") == 0)
		ret = ft_export(cmds, shell);
	else if (ft_strcmp(command, "unset") == 0)
		ret = ft_unset(cmds, shell);
	else if (ft_strcmp(command, "env") == 0)
		ret = ft_env(shell->env);
	else if (ft_strcmp(command, "exit") == 0)
	{
		ret = ft_exit(cmds->argv, shell, &shell->arena, is_child_process);
		if (is_child_process)
			exit(shell->exit_code);
	}
	return (ret);
}
