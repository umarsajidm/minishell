/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_error.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 19:04:59 by musajid           #+#    #+#             */
/*   Updated: 2025/12/17 19:04:59 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	err_if_redir_fails(t_exec *exec, t_shell *shell)
{
	clean_exec(exec);
	shell->exit_code = GENERAL_ERROR;
	return (1);
}

void	err_if_redir_fails_parent(t_exec *exec, t_shell *shell)
{
	clean_exec(exec);
	shell->exit_code = GENERAL_ERROR;
	return ;
}

void	clean_close(t_exec *exec)
{
	clean_exec(exec);
	close_fd(exec->fd);
}
