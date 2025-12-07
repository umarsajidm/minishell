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
	free(path_to_exec);
	if (arr != NULL)
		freearray(arr);
	exit(shell->exit_code);
}

void main_pipeline(t_cmd *command, t_shell *shell)
{
    pid_t pid = -1;

    init_fd(shell->fd);
	if (!command->next)
	{
        if (is_builtin(command))
        {
            // --- CRITICAL FIX 1: SAVE original FDs ---
            // Save the terminal's STDIN (0) and STDOUT (1) file descriptors
            int saved_stdin = dup(STDIN_FILENO);
            int saved_stdout = dup(STDOUT_FILENO);

            if (saved_stdin == -1 || saved_stdout == -1)
            {
                perror("minishell: dup failed");
                close_fd(shell->fd);
                if (saved_stdin != -1) close(saved_stdin);
                if (saved_stdout != -1) close(saved_stdout);
                return;
            }
            // --- End CRITICAL FIX 1 ---

            // Initialize/clear FDs before applying redirs (important for reuse)
            shell->fd->in_fd = -1;
            shell->fd->out_fd = -1;

            if (command->redirs)
                applying_redir(command->redirs, &shell->fd->in_fd, &shell->fd->out_fd);
    
            // Perform the redirection using dup2
            if (shell->fd->in_fd != -1)
                dup2(shell->fd->in_fd, STDIN_FILENO);
            if (shell->fd->out_fd != -1)
                dup2(shell->fd->out_fd, STDOUT_FILENO);
            
            // Execute the built-in
            shell->exit_code = run_builtin(command, shell);

            // --- CRITICAL FIX 2: RESTORE original FDs and CLEANUP ---

            // 1. Close the file descriptors opened by applying_redir (the temporary ones)
            close_fd(shell->fd); 
            
            // 2. Restore STDIN and STDOUT back to the original terminal FDs
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);

            // 3. Close the saved FD copies (we don't need these temporary dup-ed FDs anymore)
            close(saved_stdin);
            close(saved_stdout);
            // --- End CRITICAL FIX 2 ---
            
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
				set_the_exit_code(shell, command->argv[0], envp);
				return ;
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
		free(path_to_exec);
		freearray(envp);
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
