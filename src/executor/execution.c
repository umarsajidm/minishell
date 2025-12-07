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

static	char	*join_and_check(t_shell *shell, char *dir, char *pathcmd, char **paths)
{
	char	*path;

	path = ft_strjoin(dir, pathcmd);
	if (!path)
		set_the_code_and_exit(shell, COMMAND_NOT_FOUND, pathcmd, paths);
		// freestrnarrexit(paths, pathcmd, 1);
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

char	*pathtoexecute(t_shell *shell, char **cmd, char **env)
{
	int		i;
	char	**paths;
	char	*pathcmd;
	char	*path;

	pathcmd = ft_strjoin("/", cmd[0]);
	if (!pathcmd)
		set_the_code_and_exit(shell, GENERAL_ERROR, NULL, env);
		// strerrornexit();
	paths = get_path(env);
	if (!paths || !*paths)
		set_the_code_and_exit(shell, ENV_PATH_COULDNT_BE_FOUND, pathcmd, paths);
		// freestrnarrexit(paths, pathcmd, 127);
	i = 0;
	while (paths[i])
	{
		path = join_and_check(shell, paths[i], pathcmd, paths);
		if (path)
			return (path);
		i++;
	}

	// set_the_code_and_exit(shell, COMMAND_NOT_FOUND, pathcmd, paths);
	// freeall(paths, pathcmd, cmd[0]);
	freearray(paths);
	free(pathcmd);
	free(path);
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
static void abs_path_execution(t_cmd *cmd, t_shell *shell, char **env)
{
    checking(cmd->argv[0]);
    if (execve(cmd->argv[0], &(cmd->argv[0]), env) == -1)
    {
        // freearray(cmd->argv);
        exit_after_execve(shell, NULL, env);
    }
}

static int relative_path_execution(t_shell *shell, t_cmd *cmd, char **env, char* path_to_exec)
{

	// printf("\n in the relative path execution \n");
    if (!env)
    	set_the_code_and_exit(shell, ENVIRONMENT_COPY_FAILED, NULL, env);
    if (!cmd->argv)
        set_the_code_and_exit(shell, GENERAL_ERROR, NULL, env);
        // set_the_code_and_exit(shell, COMMAND_NOT_FOUND, NULL, env);
    checking(path_to_exec);
    if (execve(path_to_exec, cmd->argv, env) == -1)
		exit_after_execve(shell, path_to_exec, env);
	// freearray(cmd);
	return (1);
}


int	execution(t_cmd *cmd, t_shell *shell, char **env, char *path_to_exec)
{
	if (ft_strchr(cmd->argv[0], '/' ))
		abs_path_execution(cmd, shell, env);
	else
	{
		if (relative_path_execution(shell, cmd, env, path_to_exec) == 1)
			return 1;
	}
	
	freearray(env);
	free(path_to_exec);
	return (0);
}

int	child_process(t_cmd *cmd, t_shell *shell, t_fd *fd, char **env, char *path_to_exec)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return(ft_putstr_fd("forking failed in child process", 2), 1);
	if (pid == 0)
	{

		if (fds_manipulation_and_execution(cmd, shell, fd, env, path_to_exec) == 1)
			set_the_code_and_exit(shell, GENERAL_ERROR, NULL, env);
	}
	// printf("\ni am here as well in the child process\n");
	if (pid > 0)
		waitstatus(pid, shell);
	freearray(env);
	free(path_to_exec);
	return (0);
}
