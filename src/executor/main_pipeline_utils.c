/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 18:01:28 by musajid           #+#    #+#             */
/*   Updated: 2025/12/17 19:53:23 by musajid          ###   ########.fr       */
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

int	init_exec(t_exec *exec, t_shell *shell, t_cmd *command)
{
	char	*cmd_name_for_error;

	pre_init(exec);
	exec->envp = envp_arr(shell);
	if (!exec->envp)
		return (1);
	if (command->argv && command->argv[0])
		exec->path_to_exec = pathtoexecute(command->argv, exec);
	if (!exec->path_to_exec)
	{
		freearray(exec->envp);
		exec->envp = NULL;
		shell->exit_code = 127;
		if (command->argv && command->argv[0] && command->argv[0][0])
			cmd_name_for_error = command->argv[0];
		else
			cmd_name_for_error = command->unexpanded_cmd;
		if (cmd_name_for_error)
		{
			ft_putstr_fd(cmd_name_for_error, 2);
			ft_putstr_fd(": command not found\n", 2);
		}
		return (1);
	}
	return (0);
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

int	intialize_and_process_single_child(t_exec *exec,
	t_shell *shell, t_cmd *command)
{
	if (init_exec(exec, shell, command) == 0)
	{
		if (command->redirs)
		{
			if (applying_redir(command, &(exec->fd->in_fd),
					&(exec->fd->out_fd)) == 1)
				return (err_if_redir_fails(exec, shell));
		}
		child_process(command, shell, exec);
	}
	else
		return (1);
	return (0);
}

int	initialize_and_process_multiple_child(t_exec *exec,
	t_shell *shell, t_cmd *command)
{
	pipe(exec->fd->fd);
	setup_execution_signals();
	exec->pid = fork();
	if (exec->pid < 0)
	{
		perror("fork failed in multiple child process");
		setup_parent_signals();
		return (1);
	}
	if (exec->pid == 0)
	{
		setup_child_signals();
		if (!is_builtin(command) && init_exec(exec, shell, command) != 0)
			set_the_code_and_exit(shell, exec, 127);
		if (command->redirs)
		{
			if (applying_redir(command, &(exec->fd->in_fd),
					&(exec->fd->out_fd)) == 1)
				set_the_code_and_exit(shell, exec, GENERAL_ERROR);
		}
		if (fds_manipulation_and_execution(command, shell, exec) == 1)
			set_the_code_and_exit(shell, exec, COMMAND_NOT_FOUND);
		exit(0);
	}
	return (0);
}
