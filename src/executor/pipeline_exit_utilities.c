/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exit_utilities.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:09:10 by musajid           #+#    #+#             */
/*   Updated: 2025/12/17 18:09:11 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Prints an error message to stderr based on the error type.
** Uses perror to print system error details.
*/
static void	print_execution_error(int type)
{
	if (type == PERMISSION_DENIED)
		perror("minishell");
	else if (type == COMMAND_NOT_FOUND)
		perror("minishell");
	else if (type == FORK_FAILED)
		perror("minishell");
}

/*
** Frees all shell resources and exits the child process.
** Cleans up arena, env, exec struct, and readline history.
** Exits with specific codes for command not found (127)
** or permission denied (126), or the passed type for others.
*/
static void	child_cleanup_and_exit(t_shell *shell, int type)
{
	if (shell->arena)
		free_arena(&shell->arena);
	if (shell->env)
		free_env(shell->env);
	if (shell->exec)
	{
		if (shell->exec->fd)
			free(shell->exec->fd);
		free(shell->exec);
	}
	rl_clear_history();
	if (type == COMMAND_NOT_FOUND)
		exit(127);
	if (type == PERMISSION_DENIED)
		exit(126);
	exit(type);
}

/*
** Main exit handler for child process errors.
** 1. Frees execution-specific resources (envp, path).
** 2. Closes file descriptors.
** 3. Prints error message.
** 4. Calls full cleanup and exit.
*/
void	set_the_code_and_exit(t_shell *shell, t_exec *exec, int type)
{
	if (exec->envp)
		freearray(exec->envp);
	if (exec->path_to_exec != NULL)
		free(exec->path_to_exec);
	close_fd(exec->fd);
	print_execution_error(type);
	child_cleanup_and_exit(shell, type);
}

/*
** Sets the exit code for a command not found error and cleans up.
** Used in cases where we don't exit immediately but update state.
*/
void	set_the_exit_code(t_shell *shell, char *command, char **envp)
{
	if (envp != NULL)
		freearray(envp);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": command not found (set the exit code)\n", 2);
	execution_cleanup(shell);
	shell->exit_code = 127;
}

/*
** Handles exit after a failed execve call.
** Translates errno values (ENOENT, EACCES, EISDIR) into
** appropriate internal error types (COMMAND_NOT_FOUND, PERMISSION_DENIED)
** and calls the exit handler.
*/
void	exit_after_execve(t_shell *shell, t_exec *exec)
{
	if (errno == ENOENT)
		set_the_code_and_exit(shell, exec, COMMAND_NOT_FOUND);
	else if (errno == EACCES || errno == EISDIR)
		set_the_code_and_exit(shell, exec, PERMISSION_DENIED);
	else
		set_the_code_and_exit(shell, exec, GENERAL_ERROR);
}
