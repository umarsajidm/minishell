#include "minishell.h"

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
			if (!paths || !*paths)
					freeerror(paths);
			return (paths);
		}
		i++;
	}
	return (NULL);
}


static char *search_path_for_cmd(char *cmd_name, char **paths)
{
    char    *candidate;
    char    *join;
    int     i;

    i = 0;
    while (paths && paths[i])
    {
        join = ft_strjoin(paths[i], "/");
        if (!join)
            return (NULL);
        candidate = ft_strjoin(join, cmd_name);
        free(join);
        if (!candidate)
            return (NULL);
        if (access(candidate, F_OK) == 0)
            return (candidate);
        free(candidate);
        i++;
    }
    return (NULL);
}

char    *pathtoexecute(char **cmd, t_exec *exec)
{
    char    **paths;
    char    *path_to_exec;

    if (!cmd || !cmd[0] || !cmd[0][0])
        return (NULL);
    if (ft_strchr(cmd[0], '/'))
    {
        if (access(cmd[0], F_OK) == 0)
            return (ft_strdup(cmd[0]));
        return (NULL);
    }
    paths = get_path(exec->envp);
    path_to_exec = search_path_for_cmd(cmd[0], paths);
    if (paths)
        freearray(paths);
    return (path_to_exec);
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
//need serparate function for absolute path
static void abs_path_execution(t_cmd *cmd, t_shell *shell, t_exec *exec)
{
    checking(cmd->argv[0]);
    if (execve(cmd->argv[0], &(cmd->argv[0]), exec->envp) == -1)
    {
        exit_after_execve(shell, exec);
    }
}

static int relative_path_execution(t_shell *shell, t_cmd *command, t_exec *exec)
{

	// printf("\n in the relative path execution \n");
    if (!exec->envp)
    	set_the_code_and_exit(shell, exec, ENVIRONMENT_COPY_FAILED);
    if (!command->argv)
        set_the_code_and_exit(shell, exec ,GENERAL_ERROR);
    checking(exec->path_to_exec);
    if (execve(exec->path_to_exec, command->argv, exec->envp) == -1)
		exit_after_execve(shell, exec);
	return (1);
}


int	execution(t_cmd *command, t_shell *shell, t_exec *exec)
{
	if (is_builtin(command))
	{
		clean_exec(exec);
		shell->exit_code = run_builtin(command, shell, true);
		return (0);

	}
	else if (ft_strchr(command->argv[0], '/' ))
		abs_path_execution(command, shell, exec);
	else
	{
		if (relative_path_execution(shell, command, exec) == 1)
			return 1;
	}
	return (0);
}

int	child_process(t_cmd *cmd, t_shell *shell, t_exec *exec)
{
	setup_execution_signals();
	exec->pid = fork();
	if (exec->pid < 0)// handle fork failing
		return(ft_putstr_fd("forking failed in child process", 2), 1);
	if (exec->pid == 0)
	{
		setup_child_signals();
		if (fds_manipulation_and_execution(cmd, shell, exec) == 1)
			set_the_code_and_exit(shell, exec, GENERAL_ERROR);
		clean_exec(exec);
		execution_cleanup(shell);
	}
	// printf("\ni am here as well in the child process\n");
	if (exec->pid > 0)
		waitstatus(exec->pid, shell);
	setup_parent_signals();
	while (waitpid(-1, NULL, 0)>0);
	return (0);
}
