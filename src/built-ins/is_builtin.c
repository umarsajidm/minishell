/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:09:42 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/18 13:09:43 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Checks if a command is a built-in function.
** Compares the command name against known built-ins:
** echo, cd, pwd, export, unset, env, exit.
** Returns 1 if it is a built-in, 0 otherwise.
*/
int	is_builtin(t_cmd *cmd)
{
	char	*command;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
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

/*
** Checks if a command is a "parent-level" built-in.
** These commands modify the shell state (env, cwd, etc.) and
** must be executed in the parent process, not a child.
** Includes: cd, export, unset, exit.
** Returns 1 if true, 0 otherwise.
*/
int	is_parent_level_builtin(t_cmd *cmd)
{
	char	*command;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	command = cmd->argv[0];
	if (ft_strcmp(command, "cd") == 0)
		return (1);
	else if (ft_strcmp(command, "export") == 0)
		return (1);
	else if (ft_strcmp(command, "unset") == 0)
		return (1);
	else if (ft_strcmp(command, "exit") == 0)
		return (1);
	return (0);
}
