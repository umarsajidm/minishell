#include "minishell.h"

void pre_init(t_exec *exec)
{
    if (!exec)
        return;
    if (exec->path_to_exec)
    {
        free(exec->path_to_exec);
        exec->path_to_exec = NULL;
    }
    if (exec->envp)
    {
        freearray(exec->envp);
        exec->envp = NULL;
    }
    exec->pid = -1;
    exec->jonatan = 0;
}


int  init_exec(t_exec *exec, t_shell *shell, t_cmd *command)
{    
    // exec = shell->exec;
    pre_init(exec);
    // exec->fd = malloc(sizeof(t_fd));
    // if (!exec->fd)
    //     return (1); //have to write a str error
    exec->envp = envp_arr(shell);
    if (!exec->envp)
        return(1);
    if (command->argv && command->argv[0]) //need a str error
        exec->path_to_exec = pathtoexecute(command->argv, exec);
    if (!exec->path_to_exec)
        return(free(exec->envp), 1);
    //need to make a function to putste and return value;
    return (0);
}

void clean_exec(t_exec *exec)
{
    if (exec->path_to_exec != NULL)
    {
        free(exec->path_to_exec);
        exec->path_to_exec = NULL;
    }
    if (exec->envp != NULL)
    {
        freearray(exec->envp);
        exec->envp = NULL;
    }

}

int intialize_and_process_single_child(t_exec *exec, t_shell *shell, t_cmd *command)
{
    if (init_exec(exec, shell, command) == 0) //what if it fails
    {   
        if (command->redirs)
            applying_redir(command,  &(exec->fd->in_fd), &(exec->fd->out_fd));
        child_process(command, shell, exec); //where am i handling the anomaly
    }
    else 
        return (1);
    return (0);
}

int initialize_and_process_multiple_child(t_exec *exec, t_shell *shell, t_cmd *command)
{
    if (init_exec(exec, shell, command) == 0)
    {
        if (command->redirs)
            applying_redir(command,  &(exec->fd->in_fd), &(exec->fd->out_fd));
    }
    else
    {
        clean_exec(exec);
        return (1);
    }
    pipe(exec->fd->fd);
    exec->pid = fork();
    if (exec->pid < 0)
    {
        perror("fork failed in multiple child process");
        return (1);
    }
    if (exec->pid == 0)
    {
        if (fds_manipulation_and_execution(command, shell, exec) == 1)
            set_the_code_and_exit(shell, exec, COMMAND_NOT_FOUND);
    }
    return (0);
}

// void child_process_for_multiple()
void validate_command(t_exec *exec, t_shell *shell, t_cmd *command)
{
    // init_fd(exec->fd);
    //only one command (no pipeline)
    if (!command->next)
    {
        if (intialize_and_process_single_child(exec, shell, command) == 0)
        {
            clean_exec(exec);
            close_fd(exec->fd);
        }
        return;
    }
   //x multiple commands (pipeline)
    while (command)
    {
        if (initialize_and_process_multiple_child(exec, shell, command) == 1)
            clean_exec(exec);
        else
        {
            parent_loop(command, exec->fd);
            command = command->next;
        }
    }

    waitstatus(exec->pid, shell);
    while (waitpid(-1, NULL, 0) > 0);

    close_fd(exec->fd);
    clean_exec(exec);
}


