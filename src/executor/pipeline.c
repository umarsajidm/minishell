/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 19:03:08 by musajid           #+#    #+#             */
/*   Updated: 2025/12/17 19:03:08 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	waitstatus(pid_t pid, t_shell *shell);

static int	setup_builtin_redirs(t_shell *shell, t_cmd *command,
								int *saved_stdin, int *saved_stdout)
{
	if (!command->redirs)
		return (0);
	if (applying_redir(command, &shell->exec->fd->in_fd,
			&shell->exec->fd->out_fd) == 1)
		return (1);
	if (shell->exec->fd->out_fd != -1)
	{
		*saved_stdout = dup(STDOUT_FILENO);
		dup2(shell->exec->fd->out_fd, STDOUT_FILENO);
	}
	if (shell->exec->fd->in_fd != -1)
	{
		*saved_stdin = dup(STDIN_FILENO);
		dup2(shell->exec->fd->in_fd, STDIN_FILENO);
	}
	return (0);
}

static void	restore_builtin_fds(int saved_stdin, int saved_stdout,
								t_fd *fd)
{
	if (saved_stdout != -1)
		dup2(saved_stdout, STDOUT_FILENO);
	if (saved_stdin != -1)
		dup2(saved_stdin, STDIN_FILENO);
	close_fd(fd);
}

static void	handle_single_builtin(t_shell *shell, t_cmd *command)
{
	int	saved_stdout;
	int	saved_stdin;

	saved_stdout = -1;
	saved_stdin = -1;
	if (setup_builtin_redirs(shell, command,
			&saved_stdin, &saved_stdout) == 1)
	{
		shell->exit_code = GENERAL_ERROR;
		return ;
	}
	shell->exit_code = run_builtin(command, shell, false);
	restore_builtin_fds(saved_stdin, saved_stdout, shell->exec->fd);
}

void	main_pipeline(t_shell *shell, t_cmd *command)
{
	t_exec	*exec;

	exec = shell->exec;
	if (is_builtin(command) && !command->next)
	{
		handle_single_builtin(shell, command);
		if (shell->exit_flow == FLOW_EXIT)
		{
			shell->running = false;
		}
		return ;
	}
	validate_command(exec, shell, command);
}

void	waitstatus(pid_t pid, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_code = WTERMSIG(status) + 128;
}
