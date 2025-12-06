#include "execution.h"

void waitstatus(pid_t pid,  t_shell *shell);
void close_fd(t_fd *fd);
// void init_fd(t_fd    *fd);
static void parent_loop(t_cmd *cmd, t_fd *fd);

static void init_fd(t_fd    *fd)
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
int fds_manipulation_and_execution(t_cmd *cmd, t_shell *shell, t_fd *fd, char **arr, char *path_to_exec)
{
    if (fd->prev_fd != -1)
        dup2(fd->prev_fd, STDIN_FILENO);
    if (fd->in_fd != -1)
        dup2(fd->in_fd, STDIN_FILENO);
    if (cmd->next != NULL && fd->out_fd == -1)
        dup2(fd->fd[1], STDOUT_FILENO);
    if (fd->out_fd != -1)
        dup2(fd->out_fd, STDOUT_FILENO);
    close_fd(fd);

    // FIX 2: Call to execution() now includes 4 arguments (path_to_exec)
    if (execution(cmd, shell, arr, path_to_exec) == 1)
        exit_after_execve(shell, NULL, arr);

    freearray(arr);
    exit(shell->exit_code);
}

void main_pipeline(t_cmd *command, t_shell *shell)
{
    pid_t pid = -1;

    char **envp = envp_arr(shell);
    init_fd(shell->fd); // init_fd is called here

    // --- SINGLE COMMAND CASE ---
    if (!command->next)
    {
        if (is_builtin(command) && is_parent_level_builtin(command))
        {
            // FIX 3: run_builtin signature change (assuming &shell->arena was removed)
            shell->exit_code = run_builtin(command, shell);
            goto cleanup;
        }
        else if (!command->next) // This redundant 'else if' should be removed later
        {
            if (command->redirs)
                applying_redir(command->redirs, &(shell->fd->in_fd), &(shell->fd->out_fd));

            if (child_process(command, shell, shell->fd, envp) == 1)
                set_the_code_and_exit(shell, GENERAL_ERROR, NULL, envp);
        }
        goto cleanup;
    }

    // --- PIPELINE EXECUTION ---
    while (command)
    {
        pipe(shell->fd->fd);
        pid = fork();
        if (pid < 0)
        {
            perror("fork failed in child process");
            break; // Go to cleanup
        }
        else if (pid == 0)
        {
            // FIX 4: Call to fds_manipulation_and_execution now includes 5 arguments (NULL)
            if (fds_manipulation_and_execution(command, shell, shell->fd, envp, NULL) == 1)
            {
                freearray(envp);
                close_fd(shell->fd);
                return ;
            }
        }
        parent_loop(command, shell->fd); // parent_loop is called here
        command = command->next;
    }

cleanup:
    cleanup_pipeline(shell, envp, pid);
    return ;
}

void waitstatus(pid_t pid,  t_shell *shell)
{
    int status;

    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
        shell->exit_code = WEXITSTATUS(status);
}
