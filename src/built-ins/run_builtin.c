/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:09:25 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/18 13:09:32 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdbool.h>

static void	clean_exit(t_shell *shell);

int	run_builtin(t_cmd *cmds, t_shell *shell, bool is_child_process)
{
	char	*command;
	int		ret;

	ret = 0;
	command = cmds->argv[0];
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
			clean_exit(shell);
	}
	return (ret);
}

static void	clean_exit(t_shell *shell)
{
	shell_cleanup(shell);
	exit(shell->exit_code);
}
