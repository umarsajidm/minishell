/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 17:51:54 by musajid           #+#    #+#             */
/*   Updated: 2025/10/16 19:39:48 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <sys/wait.h>
# include <stdlib.h>

int		main(int ac, char **av, char **envp);
void	freeerror(char **arr);
void	strerrornexit(void);
void	freearray(char **arr);
void	freeall(char **arr, char *str, char *cmd);
void	commandnotfound(char **arr);
void	freestrnarrexit(char **arr, char *str, int i);
void	execution(char **cmd, char **envp);
int		is_builtin(char *cmd);


# endif

