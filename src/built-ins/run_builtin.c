#include "minishell.h"

void	run_builtin(t_cmd *cmds, t_shell *shell)
{
	(void)shell;
	char	*command;

	command = cmds->argv[0];
	if (ft_strcmp(command, "echo") == 0)
		ft_echo(cmds->argv);
	else if (ft_strcmp(command, "cd") == 0)
		return ;
	else if (ft_strcmp(command, "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(command, "export") == 0)
		return ;
	else if (ft_strcmp(command, "unset") == 0)
		return ;
	else if (ft_strcmp(command, "env") == 0)
		ft_env(shell->env);
	else if (ft_strcmp(command, "exit") == 0)
		ft_exit(cmds->argv, shell);
}