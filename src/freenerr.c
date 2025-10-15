/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freenerr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 14:11:25 by musajid           #+#    #+#             */
/*   Updated: 2025/08/21 15:13:21 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freearray(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void	freestrnarrexit(char **arr, char *str, int i)
{
	freearray(arr);
	free(str);
	if (i == 126)
	{
		errno = EACCES;
		exit(126);
	}
	else if (i == 127)
	{
		errno = ENOENT;
		exit(127);
	}
	exit(EXIT_FAILURE);
}

void	freeerror(char **arr)
{
	if (arr)
		freearray(arr);
	perror("error");
	exit(127);
}

void	freeall(char **arr, char *str, char *cmd)
{
	freearray(arr);
	free(str);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

void	commandnotfound(char **arr)
{
	freearray(arr);
	errno = ENOENT;
	exit(127);
}
