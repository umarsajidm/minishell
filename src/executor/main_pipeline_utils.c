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
}


int  init_exec(t_exec *exec, t_shell *shell, t_cmd *command)
{
    char *cmd_name_for_error;

    pre_init(exec);
    exec->envp = envp_arr(shell);
    if (!exec->envp)
        return(1);
    if (command->argv && command->argv[0]) //need a str error
        exec->path_to_exec = pathtoexecute(command->argv, exec);
    if (!exec->path_to_exec)
    {
		freearray(exec->envp);
		exec->envp = NULL;
        shell->exit_code = 127;
		if (command->argv && command->argv[0] && command->argv[0][0])
			cmd_name_for_error = command->argv[0];
		else
			cmd_name_for_error = command->unexpanded_cmd;
		if (cmd_name_for_error)
		{
			ft_putstr_fd(cmd_name_for_error, 2);
			ft_putstr_fd(": command not found\n", 2);
		}
        return (1);
    }
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
        {
			if (applying_redir(command,  &(exec->fd->in_fd), &(exec->fd->out_fd)) == 1)
			    return(err_if_redir_fails(exec, shell));
		}
        child_process(command, shell, exec); //where am i handling the anomaly
    }
    else
        return (1);
    return (0);
}

int initialize_and_process_multiple_child(t_exec *exec, t_shell *shell, t_cmd *command)
{
    pipe(exec->fd->fd);
    setup_execution_signals();

    exec->pid = fork();
    if (exec->pid < 0)
    {
        perror("fork failed in multiple child process");
        setup_parent_signals();
        return (1);
    }
    if (exec->pid == 0)
    {
        setup_child_signals();
        if (init_exec(exec, shell, command) != 0)
            set_the_code_and_exit(shell, exec, 127);
        if (command->redirs)
        {
            if (applying_redir(command, &(exec->fd->in_fd), &(exec->fd->out_fd)) == 1)
                set_the_code_and_exit(shell, exec, GENERAL_ERROR);
        }
        if (fds_manipulation_and_execution(command, shell, exec) == 1)
            set_the_code_and_exit(shell, exec, COMMAND_NOT_FOUND);
        exit(0);
    }
    return (0);
}

void validate_command(t_exec *exec, t_shell *shell, t_cmd *command)
{
    int error_in_pipeline = 0;

    init_fd(exec->fd);
    if (!command->next)
    {
        if (intialize_and_process_single_child(exec, shell, command) == 0)
            clear_close(exec);
        return;
    }
    while (command)
    {
        if (initialize_and_process_multiple_child(exec, shell, command) != 0)
        {
            error_in_pipeline = 1;
            break ;
        }
        parent_loop(command, exec->fd);
        command = command->next;
    }
    if (!error_in_pipeline && exec->pid > 0)
        waitstatus(exec->pid, shell);
    while (waitpid(-1, NULL, 0) > 0)
        ;
    clear_close(exec);
}
