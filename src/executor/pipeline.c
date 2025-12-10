#include "minishell.h"

void waitstatus(pid_t pid,  t_shell *shell);

void init_fd(t_fd	*fd)
{
	fd->fd[0] = -1;
	fd->fd[1] = -1;
	fd->prev_fd = -1;
	fd->in_fd = -1;
	fd->out_fd = -1;
}

void close_fd(t_fd *fd)
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

void parent_loop(t_cmd *cmd, t_fd *fd)
{
	//parent closes the prev
	if (fd->prev_fd != -1)
            close(fd->prev_fd);

        // Keep current read end for next command
    if (cmd->next != NULL)
        fd->prev_fd = fd->fd[0];
    else
        fd->prev_fd = -1;
        // Close write end in parent
    close(fd->fd[1]);
}


int	fds_manipulation_and_execution(t_cmd *command, t_shell *shell, t_exec *exec)
{
	//if there is pipe, if not then go to the execution
	if (exec->fd->prev_fd != -1)
		dup2(exec->fd->prev_fd, STDIN_FILENO);
	if (exec->fd->in_fd != -1)
		dup2(exec->fd->in_fd, STDIN_FILENO);
	if (command->next != NULL && exec->fd->out_fd == -1)
		dup2(exec->fd->fd[1], STDOUT_FILENO);
	if (exec->fd->out_fd != -1)
		dup2(exec->fd->out_fd, STDOUT_FILENO);
	close_fd(exec->fd);
	if (execution(command, shell, exec) == 1) //execve is vc
		exit_after_execve(shell, exec);
	free(exec->path_to_exec);
	if (exec->envp != NULL)
		freearray(exec->envp);
	execution_cleanup(shell);
	exit(shell->exit_code);
}

void main_pipeline(t_shell *shell, t_cmd *command)
{
	t_exec	*exec;

	exec = shell->exec;
	// init_fd(exec->fd);

	if (is_builtin(command) && !command->next)
	{
		// ft_putstr_fd("i am here", 2);
		// int saved_stdin = -1;
		// int saved_stdout = -1;
		int rc = 0;

		// saved_stdin = dup(STDIN_FILENO);
		// saved_stdout = dup(STDOUT_FILENO);
		// if (saved_stdin == -1 || saved_stdout == -1)
		// {
		// 	perror("minishell: dup failed");
		// 	rc = -1;
		// 	/* fallthrough to cleanup */
		// }
		// else if (rc == -1)
		// {
		// 	if (saved_stdin != -1)
		// 		close(saved_stdin);
		// 	if (saved_stdout != -1)
		// 		close(saved_stdout);
		// 	return ;
		// }

		/* apply redirections (may set shell->exec->fd->in_fd/out_fd) */
		if (rc == 0 && command->redirs)
		{
			if (applying_redir(command, &shell->exec->fd->in_fd, &shell->exec->fd->out_fd) == 1)
			{
				shell->exit_code = GENERAL_ERROR;
				return ;
			}
		}

		/* if redirs set fds, dup them to std fds; if dup2 fails, set rc */
		// if (rc == 0)
		// {
		// 	if (shell->exec->fd->in_fd != -1 && dup2(shell->exec->fd->in_fd, STDIN_FILENO) == -1)
		// 		rc = -1;
		// 	if (shell->exec->fd->out_fd != -1 && dup2(shell->exec->fd->out_fd, STDOUT_FILENO) == -1)
		// 		rc = -1;
		// }

		if (rc == 0)
			shell->exit_code = run_builtin(command, shell);

		/* restore original fds (always attempt) */
		// dup2(STDIN_FILENO, saved_stdin); /* ignore failure, we'll still close saved fds */
		// close(saved_stdin);

		// dup2(STDOUT_FILENO, saved_stdout);
		// close(saved_stdout);


		/* Close any opened redir fds and the shell->exec->fd structure fds */
		if (shell->exec->fd->in_fd != -1)
		{
			close(shell->exec->fd->in_fd);
			shell->exec->fd->in_fd = -1;
		}
		if (shell->exec->fd->out_fd != -1)
		{
			close(shell->exec->fd->out_fd);
			shell->exec->fd->out_fd = -1;
		}
		close_fd(shell->exec->fd);

		return;
	}
		// single or piped command execution path
	validate_command(exec, shell, command);

    

	return ;

}

void waitstatus(pid_t pid,  t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
    	shell->exit_code = WEXITSTATUS(status);
}

