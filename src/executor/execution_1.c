/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:53:34 by musajid           #+#    #+#             */
/*   Updated: 2025/12/17 18:04:37 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**get_path(char **envp_arr)
{
	int		i;
	char	**paths;

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

char	*search_path_for_cmd(char *cmd_name, char **paths)
{
	char	*candidate;
	char	*join;
	int		i;

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
		if (access(candidate, X_OK) == 0)
			return (candidate);
		free(candidate);
		i++;
	}
	return (NULL);
}

int	path_is_set(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return (0);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (1);
		i++;
	}
	return (0);
}

char	*pathtoexecute(char **cmd, t_exec *exec)
{
	char	**paths;
	char	*path;

	if (!cmd || !cmd[0] || !cmd[0][0])
		return (NULL);
	if (ft_strchr(cmd[0], '/'))
	{
		if (access(cmd[0], F_OK) == 0)
			return (ft_strdup(cmd[0]));
		return (NULL);
	}
	paths = get_path(exec->envp);
	if (paths)
	{
		path = search_path_for_cmd(cmd[0], paths);
		freearray(paths);
		if (path)
			return (path);
	}
	if (!path_is_set(exec->envp))
	{
		if (access(cmd[0], X_OK) == 0)
			return (ft_strdup(cmd[0]));
	}
	return (NULL);
}

void	checking(char *path, char *cmd)
{
	struct stat	st;

	if (!path)
	{
		errno = ENOENT;
		return ;
	}
	if (!ft_strchr(cmd, '/'))
	{
		errno = ENOENT;
		return ;
	}
	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
	{
		errno = EISDIR;
		return ;
	}
	if (access(path, X_OK) == -1)
	{
		errno = EACCES;
		return ;
	}
}
