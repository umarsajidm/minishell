/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 18:59:37 by musajid           #+#    #+#             */
/*   Updated: 2025/10/17 20:20:10 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ◦ echo with option -n
// ◦ cd with only a relative or absolute path
// ◦ pwd with no options						done
// ◦ export with no options						
// ◦ unset with no options
// ◦ env with no options or arguments			done
// ◦ exit with no options
	
static int	print_arr(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		printf("%s", str[i]);
		i++;
	}
	write(1, "\n", 1);
	return (0);
}

static int	print_pwd(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_strstr(str[i], "PWD="))
			return(printf("%s", (str[i] + 4)));
		i++;
	}
	return (0);
}
static int	print_echo(char **cmd) ///need to handle the -n option
{
    int i;

    i = 1;
    while (cmd[i])
    {
        printf("%s", cmd[i]);
        if (cmd[i + 1] != NULL) 
        {
            write(1, " ", 1);
        }
        i++;
    }
    write(1, "\n", 1); 

    return (1);
}

//what diff it make to use bool instead of int
int	is_builtin(char **cmd, char **env)
{
	if (!cmd[0])
		return (0);
	if (!ft_strcmp(cmd[0], "exit"))
		return (1);
	if (!ft_strcmp(cmd[0], "env"))
		return(print_arr(env));
	if (!ft_strcmp(cmd[0], "unset"))
		return (1);
	if (!ft_strcmp(cmd[0], "export"))
		return (1);
	if (!ft_strcmp(cmd[0], "pwd"))
		return (print_pwd(env));
	if (!ft_strcmp(cmd[0], "echo"))// need to implement with options -n
		return (print_echo(cmd));
	if (!ft_strcmp(cmd[0], "cd"))//need to implement with options of abs or relative path
		return (1);
	return (0); //need to think about this statement
}
