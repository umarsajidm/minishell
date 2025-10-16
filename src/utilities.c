/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:25:08 by musajid           #+#    #+#             */
/*   Updated: 2025/10/16 21:12:57 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//getting the path form environment



static	char	**get_path(char **env)
{
	int		i;
	char	**paths;


	i = 0;
	if (!env[i])
		return (NULL);
	while (env[i])
	{
		if (strncmp(env[i], "PATH=", 5) == 0)
		{
			paths = ft_split((env[i] + 5), ':');
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
	if ((access(path, F_OK) == 0) && (access(path, X_OK) == -1))
		errno = EACCES;
	if (access(path, X_OK) == 0)
		return ;
	if (access(path, X_OK))
		errno = ENOENT;
}
//need serparate function for absolute path

static void	abs_path_execution(char **cmd, char **env)
{
	checking(cmd[0]);
	if (execve(cmd[0], cmd, env) == -1)
	{
		freearray(cmd);
		strerrornexit();
	}

}

static void	relative_path_execution(char **cmd, char **env)
{
	char	*path;

	if (!env)
	{
		printf("copying environment failed");
		exit(EXIT_FAILURE);
	}
	if (!cmd)
		strerrornexit();
	path = pathtoexecute(cmd, env);
	if (path == NULL)
		commandnotfound(cmd);
	checking(path);
	if (execve(path, cmd, env) == -1)
	{
		freearray(cmd);
		free(path);
		strerrornexit();
	}
}

void	execution(char **cmd, char **env)
{
	if (ft_strchr(cmd[0], '/' ))
		abs_path_execution(cmd, env);
	else
		relative_path_execution(cmd, env);
}
