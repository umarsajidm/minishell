#include "execution.h"

static  char    **get_path(char **envp_arr)
{
	int             i;
	char    **paths;

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
		freestrnarrexit(paths, pathcmd, 1);
	if (access(path, X_OK) == 0)
	{
		freearray(paths);
		free(pathcmd);
		return (path);
	}
	free(path);
	return (NULL);
}

static	char	*pathtoexecute(char **cmd, char **env)
{
	int		i;
	char	**paths;
	char	*pathcmd;
	char	*path;

	pathcmd = ft_strjoin("/", cmd[0]);
	if (!pathcmd)
		strerrornexit();
	paths = get_path(env);
	if (!paths || !*paths)
		freestrnarrexit(paths, pathcmd, 127);
	i = 0;
	while (paths[i])
	{
		path = join_and_check(paths[i], pathcmd, paths);
		if (path)
			return (path);
		i++;
	}
	freeall(paths, pathcmd, cmd[0]);
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
static void abs_path_execution(t_cmd *cmd, char **env)
{
    checking(cmd->argv[0]);
    if (execve(cmd->argv[0], &(cmd->argv[0]), env) == -1)
    {
        freearray(cmd->argv);
        freearray(env);
        strerrornexit();
    }
}

// static void	notfound()
// {
// 	// void(arr);
// 	// freearray(arr);
// 	errno = ENOENT;
// 	// shell->exit_code = 127
// }

static int relative_path_execution(t_cmd *cmd, char **env)
{
    char *path;


    if (!env)
    {
        printf("copying environment failed");
        exit(EXIT_FAILURE);
    }
    if (!cmd->argv)
        strerrornexit();
    path = pathtoexecute(cmd->argv, env);
    if (path == NULL)  //ponder our hpe to deal with iin case of execve fails
        return 1;
    checking(path);
    if (execve(path, cmd->argv, env) == -1)
    {
        // freearray(cmd);
        free(path);
        freearray(env);
        strerrornexit();
    }
	return (1);
}


int	execution(t_cmd *cmd, t_shell *shell, char **env)
{
	if (ft_strchr(cmd->argv[0], '/' ))
		abs_path_execution(cmd, env);
	if (is_builtin(cmd))
		shell->exit_code = run_builtin(cmd, shell, &shell->arena);
	else
	{
		if (relative_path_execution(cmd, env) == 1)
			return(1);
	}
	return (0);
}

int	child_process(t_cmd *cmd, t_shell *shell, t_fd *fd, char **envp)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		if (execute_pipe(cmd, shell, fd, envp) == 1)
		{
			// perror("Minishell$ ");
			close_fd(fd);
			freearray(envp);
			exit(126);
		}
	}
	else if (pid > 0)
	{
		waitstatus(pid, shell);
		// freearray(envp);
	}
	else
		perror("fork");
	// freearray(envp);
	// waitstatus(pid, shell);
	return (0);
}
