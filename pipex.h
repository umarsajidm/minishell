/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:44:59 by musajid           #+#    #+#             */
/*   Updated: 2025/09/29 13:20:32 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>

typedef struct s_struct
{
	char	*path;
	int		i;
	char	**paths;
	char	**splitcmd;
	char	*cmd;
	int		found;
}	t_struct;

typedef struct s_pipe
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;
}	t_pipe;

int		main(int ac, char **av, char **envp);
void	freeerror(char **arr);
void	strerrornexit(void);
void	freearray(char **arr);
void	freeall(char **arr, char *str, char *cmd);
void	commandnotfound(char **arr);
void	freestrnarrexit(char **arr, char *str, int i);
void	execution(char *cmd, char **envp);

#endif
