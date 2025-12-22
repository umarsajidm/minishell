/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freenerr.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 17:54:38 by musajid           #+#    #+#             */
/*   Updated: 2025/12/17 17:54:39 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Frees a NULL-terminated array of strings.
** Iterates through the array freeing each string, then frees the array pointer.
*/
void	freearray(char **arr)
{
	int	i;

	if (arr == NULL)
		return ;
	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

/*
** Frees an array and a string, then exits with a specific status code.
** Used for cleaning up before exiting due to an error.
** Handles exit codes 126 (permission denied) and 127 (command not found).
** Default exit is EXIT_FAILURE.
*/
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

/*
** Frees an array if it exists, prints a system error message, and exits with 127.
*/
void	freeerror(char **arr)
{
	if (arr)
		freearray(arr);
	perror("error");
	exit(127);
}

/*
** Frees resources and prints a "command not found" error message to stderr.
** Frees the string array and the path string.
*/
void	freeall(char **arr, char *str, char *cmd)
{
	freearray(arr);
	free(str);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
}

/*
** Frees an array, sets errno to ENOENT (No such file or directory),
** and exits with EXIT_FAILURE.
*/
void	commandnotfound(char **arr)
{
	freearray(arr);
	errno = ENOENT;
	exit(EXIT_FAILURE);
}
