#include "minishell.h"

void	waitstatus(pid_t pid, t_shell *shell);

void	init_fd(t_fd	*fd)
{
	fd->fd[0] = -1;
	fd->fd[1] = -1;
	fd->prev_fd = -1;
	fd->in_fd = -1;
	fd->out_fd = -1;
}

void	close_fd(t_fd *fd)
{
	if (fd->fd[0] >= 0)
		close(fd->fd[0]);
	if (fd->fd[1] >= 0)
		close(fd->fd[1]);
	if (fd->prev_fd >= 0)
		close(fd->prev_fd);
	if (fd->in_fd >= 0)
		close(fd->in_fd);
	if (fd->out_fd >= 0)
		close(fd->out_fd);
	init_fd(fd);
}

void	parent_loop(t_cmd *cmd, t_fd *fd)
{
	if (fd->prev_fd != -1)
		close(fd->prev_fd);
	if (cmd->next != NULL)
		fd->prev_fd = fd->fd[0];
	else
		fd->prev_fd = -1;
	close(fd->fd[1]);
}

int	fds_manipulation_and_execution(t_cmd *command,
	t_shell *shell, t_exec *exec)
{
	if (exec->fd->prev_fd != -1)
		dup2(exec->fd->prev_fd, STDIN_FILENO);
	if (exec->fd->in_fd != -1)
		dup2(exec->fd->in_fd, STDIN_FILENO);
	if (command->next != NULL && exec->fd->out_fd == -1)
		dup2(exec->fd->fd[1], STDOUT_FILENO);
	if (exec->fd->out_fd != -1)
		dup2(exec->fd->out_fd, STDOUT_FILENO);
	close_fd(exec->fd);
	if (execution(command, shell, exec) == 1)
		exit_after_execve(shell, exec);
	free(exec->path_to_exec);
	if (exec->envp != NULL)
		freearray(exec->envp);
	execution_cleanup(shell);
	exit(shell->exit_code);
}

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

static void restore_builtin_fds(int saved_stdin, int saved_stdout,
								t_fd *fd)
{
	if (saved_stdout != -1)
		dup2(saved_stdout, STDOUT_FILENO);
	if (saved_stdin != -1)
		dup2(saved_stdin, STDIN_FILENO);
	close_fd(fd);
}

static void handle_single_builtin(t_shell *shell, t_cmd *command)
{
	int saved_stdout = -1;
	int saved_stdin = -1;

	if (setup_builtin_redirs(shell, command,
			&saved_stdin, &saved_stdout) == 1)
	{
		shell->exit_code = GENERAL_ERROR;
		return;
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
