#include "minishell.h"

void waitstatus(pid_t pid,  t_shell *shell);

static void init_fd(t_fd	*fd)
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
}

static void parent_loop(t_cmd *cmd, t_fd *fd)
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

int	fds_manipulation_and_execution(t_cmd *cmd, t_shell *shell, t_fd *fd, char **arr, char *path_to_exec)
{
	//if there is pipe, if not then go to the execution
	if (fd->prev_fd != -1)
		dup2(fd->prev_fd, STDIN_FILENO);
	if (fd->in_fd != -1)
		dup2(fd->in_fd, STDIN_FILENO);
	if (cmd->next != NULL && fd->out_fd == -1)
		dup2(fd->fd[1], STDOUT_FILENO);
	if (fd->out_fd != -1)
		dup2(fd->out_fd, STDOUT_FILENO);
	close_fd(fd);
	if (execution(cmd, shell, arr, path_to_exec) == 1)
		exit_after_execve(shell, NULL, arr);
	// free(path_to_exec);
	// if (arr != NULL)
	// 	freearray(arr);
	exit(shell->exit_code);
}

void main_pipeline(t_cmd *command, t_shell *shell)
{
    pid_t pid = -1;

    init_fd(shell->fd);
	if (!command->next)
	{
		if (is_builtin(command) && is_parent_level_builtin(command))
		{
			// execution_cleanup(shell, envp);
			shell->exit_code = run_builtin(command, shell);
			close_fd(shell->fd);
			return ;
		}
		else if (!command->next)
		{
			// execution_cleanup(shell, envp);
			if (is_builtin(command))
			{
				shell->exit_code = run_builtin(command, shell);
				return;
			}
			char **envp = envp_arr(shell);
			char *path_to_exec = pathtoexecute(shell, command->argv, envp);
			if (!path_to_exec)
			{
				set_the_exit_code(shell, command->argv[0], envp);
				return;
				// set_the_code_and_exit(shell, COMMAND_NOT_FOUND, path_to_exec, envp);
			}
			if (command->redirs)
				applying_redir(command->redirs, &(shell->fd->in_fd), &(shell->fd->out_fd));
			if (child_process(command, shell, shell->fd, envp, path_to_exec) == 1)
				set_the_code_and_exit(shell, GENERAL_ERROR, NULL, envp);
			
		}
		// printf("\ni am here in main pipeline\n");
		close_fd(shell->fd);
		// freearray(envp);
		// exit(shell->exit_code);
		return;
	}
	while (command)
	{
		char **envp = envp_arr(shell);
		char *path_to_exec = pathtoexecute(shell, command->argv, envp);
		if (!path_to_exec)
			{
				set_the_code_and_exit(shell, COMMAND_NOT_FOUND, path_to_exec, envp);
			}
		if (command->redirs)
			applying_redir(command->redirs, &(shell->fd->in_fd), &(shell->fd->out_fd));
		pipe(shell->fd->fd);
		pid = fork();
		if (pid < 0)
			perror("fork failed in child process");
		if (pid == 0)
		{
			if (fds_manipulation_and_execution(command, shell, shell->fd, envp, path_to_exec) == 1)
			{
				freearray(envp);
				free(path_to_exec);
				close_fd(shell->fd);
				return ;
			}
		}
		parent_loop(command, shell->fd);
		command = command->next;
	}
	if (shell->fd != NULL)
		close_fd(shell->fd);
	if (pid > 0)
		waitstatus(pid, shell);
	return ;
}

void waitstatus(pid_t pid,  t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
    	shell->exit_code = WEXITSTATUS(status);

}
