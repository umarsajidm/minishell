#include "minishell.h"

int	run_builtin(t_cmd *cmds, t_shell *shell)
{
	char	*command;

	command = cmds->argv[0];
	if (ft_strcmp(command, "echo") == 0)
		return (ft_echo(cmds->argv));
	else if (ft_strcmp(command, "cd") == 0)
		return (0);
	else if (ft_strcmp(command, "pwd") == 0)
		return (ft_pwd());
	else if (ft_strcmp(command, "export") == 0)
		return (0);
	else if (ft_strcmp(command, "unset") == 0)
		return (ft_unset(cmds, shell));
	else if (ft_strcmp(command, "env") == 0)
		return (ft_env(shell->env));
	else if (ft_strcmp(command, "exit") == 0)
		return (ft_exit(cmds->argv, shell));
	printf("Could not execute built-in\n");
	return (1);
}