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
