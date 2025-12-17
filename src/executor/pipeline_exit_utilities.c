#include "minishell.h"

static void print_execution_error(int type)
{
    if (type == PERMISSION_DENIED)
        perror("minishell");
    else if (type == COMMAND_NOT_FOUND)
        perror("minishell");
    else if (type == FORK_FAILED)
        perror("minishell");
}

static void child_cleanup_and_exit(t_shell *shell, int type)
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

void set_the_code_and_exit(t_shell *shell, t_exec *exec, int type)
{
    if (exec->envp)
        freearray(exec->envp);
    if (exec->path_to_exec != NULL)
        free(exec->path_to_exec);
    close_fd(exec->fd);
    print_execution_error(type);
    child_cleanup_and_exit(shell, type);
}

void set_the_exit_code(t_shell *shell, char *command, char **envp)
{
	if (envp != NULL)
		freearray(envp);
	ft_putstr_fd(command, 2);
	ft_putstr_fd(": command not found (set the exit code)\n", 2);
    execution_cleanup(shell);
	shell->exit_code = 127;
}

void exit_after_execve(t_shell *shell, t_exec *exec)
{
    if (errno == ENOENT)
        set_the_code_and_exit(shell, exec, COMMAND_NOT_FOUND);
    else if (errno == EACCES || errno == EISDIR)
        set_the_code_and_exit(shell, exec, PERMISSION_DENIED);
    else
        set_the_code_and_exit(shell, exec, GENERAL_ERROR);
}



