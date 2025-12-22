/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:53:37 by musajid           #+#    #+#             */
/*   Updated: 2025/12/17 17:53:38 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Handles execution of commands specified with an absolute path.
** Validates the path using checking() and calls execve().
** Exits with appropriate error status if execve fails.
*/
static void	abs_path_execution(t_cmd *cmd, t_shell *shell, t_exec *exec)
{
	checking(cmd->argv[0], cmd->argv[0]);
	if (execve(cmd->argv[0], &(cmd->argv[0]), exec->envp) == -1)
	{
		exit_after_execve(shell, exec);
	}
}

/*
** Handles execution of commands that require path resolution.
** Validates environment and command arguments.
** Checks the resolved path using checking() and calls execve().
** Exits with error status if execve fails.
*/
static int	relative_path_execution(t_shell *shell,
	t_cmd *command, t_exec *exec)
{
	if (!exec->envp)
		set_the_code_and_exit(shell, exec, ENVIRONMENT_COPY_FAILED);
	if (!command->argv)
		set_the_code_and_exit(shell, exec, GENERAL_ERROR);
	checking(exec->path_to_exec, command->argv[0]);
	if (execve(exec->path_to_exec, command->argv, exec->envp) == -1)
		exit_after_execve(shell, exec);
	return (1);
}

/*
** Main execution dispatcher.
** 1. Checks if the command is a builtin and runs it if so.
** 2. If command contains '/', executes as absolute path.
** 3. Otherwise, delegates to relative path execution logic.
** Returns 0 on successful execution dispatch, 1 on failure.
*/
int	execution(t_cmd *command, t_shell *shell, t_exec *exec)
{
	if (is_builtin(command))
	{
		clean_exec(exec);
		shell->exit_code = run_builtin(command, shell, true);
		return (0);
	}
	else if (ft_strchr(command->argv[0], '/' ))
		abs_path_execution(command, shell, exec);
	else
	{
		if (relative_path_execution(shell, command, exec) == 1)
			return (1);
	}
	return (0);
}

/*
** Manages the creation of a child process for command execution.
** 1. Sets up execution signals.
** 2. Forks a new process.
** 3. Child process: sets up child signals, manipulates FDs, and executes command.
** 4. Parent process: waits for the specific child and handles process status.
** Returns 1 if fork fails, 0 otherwise.
*/
int	child_process(t_cmd *cmd, t_shell *shell, t_exec *exec)
{
	setup_execution_signals();
	exec->pid = fork();
	if (exec->pid < 0)
		return (ft_putstr_fd("forking failed in child process", 2), 1);
	if (exec->pid == 0)
	{
		setup_child_signals();
		if (fds_manipulation_and_execution(cmd, shell, exec) == 1)
			set_the_code_and_exit(shell, exec, GENERAL_ERROR);
		clean_exec(exec);
		execution_cleanup(shell);
	}
	if (exec->pid > 0)
		waitstatus(exec->pid, shell);
	setup_parent_signals();
	while (waitpid(-1, NULL, 0) > 0)
		;
	return (0);
}
