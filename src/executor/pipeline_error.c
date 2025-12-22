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

/*
** Handles errors when redirection fails during execution initialization.
** Cleans execution resources and sets shell exit code to general error.
** Returns 1 to indicate failure.
*/
int	err_if_redir_fails(t_exec *exec, t_shell *shell)
{
	clean_exec(exec);
	shell->exit_code = GENERAL_ERROR;
	return (1);
}

/*
** Handles redirection errors specifically in the parent process context.
** Cleans execution resources and sets exit code.
*/
void	err_if_redir_fails_parent(t_exec *exec, t_shell *shell)
{
	clean_exec(exec);
	shell->exit_code = GENERAL_ERROR;
	return ;
}

/*
** Helper function to perform full cleanup.
** Frees execution resources and closes all file descriptors.
*/
void	clean_close(t_exec *exec)
{
	clean_exec(exec);
	close_fd(exec->fd);
}
