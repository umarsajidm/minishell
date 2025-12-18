/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_command.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:02:04 by musajid           #+#    #+#             */
/*   Updated: 2025/12/17 19:53:34 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	validate_command(t_exec *exec, t_shell *shell, t_cmd *command)
{
	int	error_in_pipeline;

	error_in_pipeline = 0;
	init_fd(exec->fd);
	if (!command->next)
	{
		if (intialize_and_process_single_child(exec, shell, command) == 0)
			clean_close(exec);
		return ;
	}
	while (command)
	{
		if (initialize_and_process_multiple_child(exec, shell, command) != 0)
		{
			error_in_pipeline = 1;
			break ;
		}
		parent_loop(command, exec->fd);
		command = command->next;
	}
	if (!error_in_pipeline && exec->pid > 0)
		waitstatus(exec->pid, shell);
	while (waitpid(-1, NULL, 0) > 0)
		;
	clean_close(exec);
}
