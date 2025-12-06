#include "minishell.h"

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
		set_the_code_and_exit(COMMAND_NOT_FOUND, pathcmd, paths);
		// freestrnarrexit(paths, pathcmd, 1);
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
		set_the_code_and_exit(GENERAL_ERROR, NULL, env);
		// strerrornexit();
	paths = get_path(env);
	if (!paths || !*paths)
		set_the_code_and_exit(ENV_PATH_COULDNT_BE_FOUND, pathcmd, paths);
		// freestrnarrexit(paths, pathcmd, 127);
	i = 0;
	while (paths[i])
	{
		path = join_and_check(paths[i], pathcmd, paths);
		if (path)
			return (path);
		i++;
	}
	set_the_code_and_exit(COMMAND_NOT_FOUND, pathcmd, paths);
	// freeall(paths, pathcmd, cmd[0]);
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
        // freearray(cmd->argv);
        exit_after_execve(NULL, env);
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
    	set_the_code_and_exit(ENVIRONMENT_COPY_FAILED, NULL, env);
    if (!cmd->argv)
        set_the_code_and_exit(GENERAL_ERROR, NULL, env);
    path = pathtoexecute(cmd->argv, env);
    if (path == NULL)  //ponder our hpe to deal with iin case of execve fails
        set_the_code_and_exit(COMMAND_NOT_FOUND, NULL, env);
    checking(path);
    if (execve(path, cmd->argv, env) == -1)
		exit_after_execve(path, env);
	// freearray(cmd);
	return (1);
}


int	execution(t_cmd *cmd, t_shell *shell, char **env)
{
	if (ft_strchr(cmd->argv[0], '/' ))
		abs_path_execution(cmd, env);
	if (is_builtin(cmd))
		shell->exit_code = run_builtin(cmd, shell);
	else
	{
		if (relative_path_execution(cmd, env) == 1)
			return 1;
	}
	return (0);
}

int	child_process(t_cmd *cmd, t_shell *shell, t_fd *fd, char **env)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{

		if (execute_pipe(cmd, shell, fd, env) == 1)
			set_the_code_and_exit(GENERAL_ERROR, NULL, env);
	}
	else if (pid > 0)
	{
		waitstatus(pid, shell);
		// freearray(envp);
	}
	else
		perror("fork");
	// freearray(env);
	//waitstatus(pid, shell);
	return (0);
}
