#include "execution.h"

void set_the_code_and_exit(t_shell *shell, int type, char *str, char **array)
{
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
	if (shell->fd != NULL)	
		close_fd(shell->fd);
	freearray(array);
	if (str != NULL)
		free(str);
	exit(type);
}

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

