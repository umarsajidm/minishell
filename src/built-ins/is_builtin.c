#include "minishell.h"

int	is_builtin(t_cmd *cmd)
{
	char	*command;

	// ft_putstr_fd("hi i am  in builtin\n", 2);
	command = cmd->argv[0];
	if (ft_strcmp(command, "echo") == 0)
		return (1);
	else if (ft_strcmp(command, "cd") == 0)
		return (1);
	else if (ft_strcmp(command, "pwd") == 0)
		return (1);
	else if (ft_strcmp(command, "export") == 0)
		return (1);
	else if (ft_strcmp(command, "unset") == 0)
		return (1);
	else if (ft_strcmp(command, "env") == 0)
		return (1);
	else if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}

int	is_parent_level_builtin(t_cmd *cmd)
{
	char	*command;

	command = cmd->argv[0];
	// if (ft_strcmp(command, "echo") == 0)
	// 	return (1);
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	// else if (ft_strcmp(command, "pwd") == 0)
	// 	return (1);
	else if (ft_strcmp(command, "export") == 0)
		return (1);
	else if (ft_strcmp(command, "unset") == 0)
		return (1);
	// else if (ft_strcmp(command, "env") == 0)
	// 	return (1);
	else if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}
