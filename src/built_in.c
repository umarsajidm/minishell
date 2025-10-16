/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 18:59:37 by musajid           #+#    #+#             */
/*   Updated: 2025/10/16 19:47:10 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// ◦ echo with option -n
// ◦ cd with only a relative or absolute path
// ◦ pwd with no options
// ◦ export with no options
// ◦ unset with no options
// ◦ env with no options or arguments
// ◦ exit with no options



//what diff it make to use bool instead of int
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "echo"))// need to implement with options -n
		return (1);
	if (!ft_strcmp(cmd, "cd"))//need to implement with options of abs or relative path
		return (1);
	return (0); //need to think about this statement
}
