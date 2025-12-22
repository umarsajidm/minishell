/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preexec_clean.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 22:33:52 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 22:33:52 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Pre-initializes the execution structure.
** Frees any existing path or environment array and resets PID.
** Ensures a clean state before new execution parameters are set.
*/
void	pre_init(t_exec *exec)
{
	if (!exec)
		return ;
	if (exec->path_to_exec)
	{
		free(exec->path_to_exec);
		exec->path_to_exec = NULL;
	}
	if (exec->envp)
	{
		freearray(exec->envp);
		exec->envp = NULL;
	}
	exec->pid = -1;
}

/*
** Cleans up memory allocated within the execution structure.
** Frees the executable path and environment array if they exist.
*/
void	clean_exec(t_exec *exec)
{
	if (exec->path_to_exec != NULL)
	{
		free(exec->path_to_exec);
		exec->path_to_exec = NULL;
	}
	if (exec->envp != NULL)
	{
		freearray(exec->envp);
		exec->envp = NULL;
	}
}
