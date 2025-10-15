/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:25:08 by musajid           #+#    #+#             */
/*   Updated: 2025/08/21 15:03:11 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//getting the path form environment
static	char	**get_path(char **envp)
{
	int		i;
	char	**paths;

	i = 0;
	if (!envp[i])
		return (NULL);
	while (envp[i])
	{
		if (strncmp(envp[i], "PATH=", 5) == 0)
		{
			paths = ft_split((envp[i] + 5), ':');
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

static	char	*pathtoexecute(char **cmd, char **envp)
{
	int		i;
	char	**paths;
	char	*pathcmd;
	char	*path;

	pathcmd = ft_strjoin("/", cmd[0]);
	if (!pathcmd)
		strerrornexit();
	paths = get_path(envp);
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

void	execution(char *cmd, char **envp)
{
	char	*path;
	char	**splitcmd;

	if (!cmd)
		strerrornexit();
	splitcmd = ft_split(cmd, ' ');
	if (!splitcmd || !*splitcmd)
		freeerror(splitcmd);
	path = pathtoexecute(splitcmd, envp);
	if (path == NULL)
		commandnotfound(splitcmd);
	checking(path);
	if (execve(path, splitcmd, envp) == -1)
	{
		freearray(splitcmd);
		free(path);
		strerrornexit();
	}
}
