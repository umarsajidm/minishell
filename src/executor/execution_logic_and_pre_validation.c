#include "execution.h"

// --- Path Finding Helpers (NO EXIT HERE) ---

static  char    **get_path(char **envp_arr)
{
    int             i;
    char    **paths;

    if (!envp_arr)
        return (NULL);
    i = 0;
    if (!envp_arr[i])
            return (NULL);
    while (envp_arr[i])
    {
        if (strncmp(envp_arr[i], "PATH=", 5) == 0)
        {
            paths = ft_split((envp_arr[i] + 5), ':');
            // NOTE: We don't call freeerror/exit here, failure is reported as NULL later.
            if (!paths || !*paths)
            {
                freearray(paths);
                return (NULL);
            }
            return (paths);
        }
        i++;
    }
    return (NULL);
}

// NOTE: This helper only frees its own memory (pathcmd, paths) and returns NULL on failure.
static  char    *join_and_check(char *dir, char *pathcmd, char **paths)
{
    char    *path;

    path = ft_strjoin(dir, pathcmd);
    if (!path)
    {
        // Allocation failure during join is fatal, exit is safer here.
        // If ft_strjoin is guaranteed safe by libft, then proceed.
        // Assuming no exit for now to allow pathtoexecute to handle errors.
        freearray(paths);
        free(pathcmd);
        return (NULL);
    }
    if (access(path, X_OK) == 0)
    {
        freearray(paths);
        free(pathcmd);
        return (path);
    }
    free(path);
    return (NULL);
}

// CRITICAL CHANGE: This function now returns the path string OR NULL if not found/error.
// It performs all necessary internal cleanup (paths, pathcmd) but does NOT exit the shell.
static  char    *pathtoexecute(char **cmd, char **env)
{
    int     i;
    char    **paths;
    char    *pathcmd;
    char    *path;

    // Check 0: Arguments
    if (!cmd || !cmd[0])
        return (NULL);

    pathcmd = ft_strjoin("/", cmd[0]);
    if (!pathcmd)
        return (NULL); // Malloc failure, handle in caller

    paths = get_path(env);
    if (!paths || !*paths)
    {
        free(pathcmd);
        // Special case: PATH not found is often error code 127 in itself.
        // We'll let the caller decide how to handle command not found.
        return (NULL);
    }

    i = 0;
    while (paths[i])
    {
        // join_and_check performs cleanup on paths and pathcmd upon success/failure
        path = join_and_check(paths[i], pathcmd, paths);
        if (path)
            return (path);
        i++;
    }

    // Cleanup: If loop finishes, paths and pathcmd are cleaned by join_and_check on failure.
    // If not, we free them now. (join_and_check should handle paths cleanup)
    freearray(paths);
    free(pathcmd);

    return (NULL); // Command not found
}

 static	void	checking(char *path)
{
	// printf("\ni am setting the status here\n");
	if ((access(path, F_OK) == 0) && (access(path, X_OK) == -1))
		errno = EACCES;
	if (access(path, X_OK) == 0)
		return ;
	if (access(path, X_OK))
		errno = ENOENT;
}


// --- Execution Helpers (Use Pre-validated Path) ---

static void abs_path_execution(t_cmd *cmd, t_shell *shell, char **env)
{
    checking(cmd->argv[0]);
    if (execve(cmd->argv[0], &(cmd->argv[0]), env) == -1)
    {
        // Failure to execve requires exit
        exit_after_execve(shell, NULL, env);
    }
}

// This function is now simplified and receives the path from the caller.
static int relative_path_execution_final(t_shell *shell, t_cmd *cmd, char **env, char *path)
{
    checking(path);
    if (execve(path, cmd->argv, env) == -1)
        exit_after_execve(shell, path, env);

    return (1); // Unreachable, but signals execution attempt
}


/* * The main execution wrapper (runs in CHILD process).
 * This function now assumes we have a pre-validated path string OR it's a builtin.
 */
int execution(t_cmd *cmd, t_shell *shell, char **env, char *path_to_exec)
{
    // 1. Absolute Path Execution (only if path was not pre-validated)
    if (path_to_exec == NULL && ft_strchr(cmd->argv[0], '/' ))
        abs_path_execution(cmd, shell, env); // Calls execve/exit

    // 2. Builtin Execution
    if (is_builtin(cmd))
    {
        shell->exit_code = run_builtin(cmd, shell);
    }

    // 3. Relative Path Execution (using the pre-validated path)
    else if (path_to_exec)
    {
        relative_path_execution_final(shell, cmd, env, path_to_exec);
    }

    // NOTE: This returns 0 for successful builtin, 1 if path_to_exec was expected but NULL (error).
    return (path_to_exec == NULL);
}

/* * The fork wrapper (runs in PARENT process).
 * This function is now responsible for validation and conditional forking.
 */
int child_process(t_cmd *cmd, t_shell *shell, t_fd *fd, char **env)
{
    pid_t   pid;
    char    *path_to_exec = NULL;

    // 1. Check if it's a non-absolute, non-builtin command
    if (!is_builtin(cmd) && !ft_strchr(cmd->argv[0], '/'))
    {
        // 1a. Validate path in the PARENT SHELL PROCESS (NO EXIT)
        path_to_exec = pathtoexecute(cmd->argv, env);

        if (path_to_exec == NULL)
        {
            // Command not found in PATH. Display error and set exit code in parent.
            ft_putstr_fd(cmd->argv[0], 2);
            ft_putstr_fd(": command not found\n", 2);
            shell->exit_code = COMMAND_NOT_FOUND; // 127
            // IMPORTANT: Temporary 'env' array is not freed here, it is freed
            // by the centralized 'cleanup_pipeline' in the parent.
            return (0); // Do NOT fork, command handled.
        }
    }
    // Note: Builtins and absolute paths skip this validation.

    // 2. Fork if execution is required (Non-parent builtin, or any external command)
    pid = fork();
    if (pid < 0)
    {
        // Cleanup path_to_exec on fork failure
        free(path_to_exec);
        return(ft_putstr_fd("forking failed in child process", 2), 1);
    }

    if (pid == 0) // Child process
    {
        // Child executes the command, handles FDs, and exits.
        if (fds_manipulation_and_execution(cmd, shell, fd, env, path_to_exec) == 1)
        {
            // Should be unreachable due to execution calling execve/exit
            free(path_to_exec);
            set_the_code_and_exit(shell, GENERAL_ERROR, NULL, env);
        }
    }

    // Parent process (pid > 0)
    if (pid > 0)
    {
        // Parent must free the path string it allocated for the child
        free(path_to_exec);
        waitstatus(pid, shell);
    }

    return (0);
}
