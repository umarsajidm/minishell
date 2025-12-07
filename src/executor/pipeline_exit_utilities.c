#include "minishell.h"

void set_the_code_and_exit(t_shell *shell, t_exec *exec, int type)
{
	// 1. Clean up temporary memory allocated in the path search or execve
	freearray(exec->envp);
	if (exec->path_to_exec != NULL)
		free(exec->path_to_exec);

    // 2. Close file descriptors inherited/opened in the child
	close_fd(exec->fd);

    // 3. Print error message
	if (type == PERMISSION_DENIED)
		perror("permission denied");
	else if (type == COMMAND_NOT_FOUND)
		perror("command not found");
	else if (type == ENV_PATH_COULDNT_BE_FOUND)
		perror("retrievinig path from env failed");
	else if (type == ENVIRONMENT_COPY_FAILED)
		perror("copying env for child process failed");
	else if (type == FORK_FAILED)
		perror("forking failed");

	printf("set the code and exit\n");
	shell->exit_code = 127;
	exit(type);
}

void set_the_exit_code(t_shell *shell, char *command, char **envp)
{
	if (envp != NULL)
		freearray(envp);
	
	// free(command);

	// if (shell->fd != NULL)
	// 	free(shell->fd);
	
	// close_fd(shell->fd);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": command not found (set the exit code)\n", 2);
	shell->exit_code = 127;
	// exit(shell->exit_code);
}

// ... exit_after_execve and cleanup_pipeline remain the same ...

void exit_after_execve(t_shell *shell, t_exec *exec)
{
	if (errno == ENOENT)
		set_the_code_and_exit(shell, exec, COMMAND_NOT_FOUND);
	else if (errno == ENOENT)
		set_the_code_and_exit(shell, exec, PERMISSION_DENIED);
	else
		set_the_code_and_exit(shell, exec, GENERAL_ERROR);
}

// void cleanup_pipeline(t_shell *shell, char **envp, pid_t last_pid)
// {
//     // 1. Close any remaining file descriptors in the parent shell
//     if (shell->fd != NULL)
//         close_fd(shell->fd);

//     // 2. Wait for the last process to finish and set the exit code
//     if (last_pid > 0)
//         waitstatus(last_pid, shell);

//     // 3. Free the environment copy allocated at the start of the function (FIXES LEAK)
//     if (envp != NULL)
//         freearray(envp);
// }

