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

static	char	*join_and_check(char *dir, char *pathcmd, char **paths)
{
	char	*path;

	path = ft_strjoin(dir, pathcmd);
	if (!path)
		return (NULL);
	if (access(path, X_OK) == 0)
	{
		freearray(paths);
		free(pathcmd);
		return (path);
	}
	// printf("in the strjoin and check");
	// free(pathcmd);
	free(path);
	return (NULL);
}

char	*pathtoexecute(char **cmd, t_exec *exec)
{
	int		i;
	char	**paths;
	char	*pathcmd;
	char	*path;

	pathcmd = ft_strjoin("/", cmd[0]);
	if (!pathcmd)
		return (NULL);
	paths = get_path(exec->envp);
	if (!paths || !*paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = join_and_check(paths[i], pathcmd, paths);
		if (path)
			return (path);
		i++;
	}
	freearray(paths);
	free(pathcmd);
	// free(path);
	return (NULL);
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
		shell->exit_code = run_builtin(command, shell);
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
	exec->pid = fork();
	if (exec->pid < 0)
		return(ft_putstr_fd("forking failed in child process", 2), 1);
	if (exec->pid == 0)
	{

		if (fds_manipulation_and_execution(cmd, shell, exec) == 1)
			set_the_code_and_exit(shell, exec, GENERAL_ERROR);
	}
	// printf("\ni am here as well in the child process\n");
	if (exec->pid > 0)
		waitstatus(exec->pid, shell);
	// while (waitpid(-1, NULL, 0)>0);
	clean_exec(exec);
	return (0);
}




// {
// 		char **envp = envp_arr(shell);
// 		char *path_to_exec = pathtoexecute(shell, command->argv, envp);
// 		if (!path_to_exec)
// 			{
// 				set_the_exit_code(shell, command->argv[0], envp);
// 				return ;
// 			}
// 		if (command->redirs)
// 			applying_redir(command, &(shell->fd->in_fd), &(shell->fd->out_fd));
// 		pipe(shell->fd->fd);
// 		pid = fork();
// 		if (pid < 0)
// 			perror("fork failed in child process");
// 		if (pid == 0)
// 		{
// 			if (fds_manipulation_and_execution(command, shell, shell->fd, envp, path_to_exec) == 1)
// 			{
// 				freearray(envp);
// 				free(path_to_exec);
// 				close_fd(shell->fd);
// 				return ;
// 			}
// 		}
// 		free(path_to_exec);
// 		freearray(envp);
// 		parent_loop(command, shell->fd);
// 		command = command->next;
// 	}
