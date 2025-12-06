#include "minishell.h"

void set_the_code_and_exit(t_shell *shell, int type, char *str, char **array)
{
	// 1. Clean up temporary memory allocated in the path search or execve
	freearray(array);
	if (str != NULL)
		free(str);

    // 2. Close file descriptors inherited/opened in the child
	if (shell->fd != NULL)
		close_fd(shell->fd);

    // CRITICAL FIX: Removed calls to free(shell->fd), arena_clear, and free_env.
    // These must ONLY happen in main.c.

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

	exit(type);
}

void set_the_exit_code(t_shell *shell, char *command, char **envp)
{
	if (envp != NULL)
		freearray(envp);

	// if (shell->fd != NULL)
	// 	free(shell->fd);
	
	// close_fd(shell->fd);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": command not found\n", 2);
	shell->exit_code = 127;
}

// ... exit_after_execve and cleanup_pipeline remain the same ...

void exit_after_execve(t_shell *shell, char *str, char **array)
{
	if (errno == ENOENT)
		set_the_code_and_exit(shell, COMMAND_NOT_FOUND, str, array);
	else if (errno == ENOENT)
		set_the_code_and_exit(shell, PERMISSION_DENIED, str, array);
	else
		set_the_code_and_exit(shell, GENERAL_ERROR, str, array);
}

void cleanup_pipeline(t_shell *shell, char **envp, pid_t last_pid)
{
    // 1. Close any remaining file descriptors in the parent shell
    if (shell->fd != NULL)
        close_fd(shell->fd);

    // 2. Wait for the last process to finish and set the exit code
    if (last_pid > 0)
        waitstatus(last_pid, shell);

    // 3. Free the environment copy allocated at the start of the function (FIXES LEAK)
    if (envp != NULL)
        freearray(envp);
}

