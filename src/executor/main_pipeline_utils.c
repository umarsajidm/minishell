/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:13:40 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 22:34:10 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Handles the child process logic for a command within a pipeline.
** 1. Sets up child signals.
** 2. Applies redirections.
** 3. Initializes execution (path resolution) if it's not a builtin.
** 4. Executes the command or manipulates file descriptors.
** Exits with appropriate status code.
*/
static int	child_process_multiple_pipeline(t_exec *exec,
			t_shell *shell, t_cmd *command)
{
	setup_child_signals();
	if (command->redirs)
	{
		if (applying_redir(command, &(exec->fd->in_fd),
				&(exec->fd->out_fd)) == 1)
			set_the_code_and_exit(shell, exec, GENERAL_ERROR);
	}
	if (command->argv && command->argv[0])
	{
		if (!is_builtin(command) && init_exec(exec, shell, command) != 0)
			set_the_code_and_exit(shell, exec, 127);
	}
	else
		shell->exit_code = 0;
	fds_manipulation_and_execution(command, shell, exec);
	exit(0);
}

/*
** Handles errors that occur during execution initialization.
** Frees environment array, sets exit code to 127, and prints error message.
** Used when path resolution fails or command is not found.
*/
static void	handle_init_exec_error(t_exec *exec, t_shell *shell, t_cmd *command)
{
	char	*cmd_name_for_error;

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
}

/*
** Initializes the execution context for a command.
** 1. Resets execution struct.
** 2. Creates environment variable array.
** 3. Resolves the executable path.
** Returns 0 on success, 1 on failure.
*/
int	init_exec(t_exec *exec, t_shell *shell, t_cmd *command)
{
	pre_init(exec);
	exec->envp = envp_arr(shell);
	if (!exec->envp)
		return (1);
	if (command->argv && command->argv[0])
		exec->path_to_exec = pathtoexecute(command->argv, exec);
	if (!exec->path_to_exec)
	{
		handle_init_exec_error(exec, shell, command);
		return (1);
	}
	return (0);
}

/*
** Orchestrates execution for a single command (no pipeline).
** 1. Applies redirections.
** 2. Initializes execution.
** 3. Spawns child process to run command.
** Returns 0 on success, 1 on failure.
*/
int	intialize_and_process_single_child(t_exec *exec,
	t_shell *shell, t_cmd *command)
{
	if (command->redirs)
	{
		if (applying_redir(command, &(exec->fd->in_fd),
				&(exec->fd->out_fd)) == 1)
			return (err_if_redir_fails(exec, shell));
	}
	if (command->argv && command->argv[0])
	{
		if (init_exec(exec, shell, command) != 0)
			return (1);
	}
	else
		shell->exit_code = 0;
	if (child_process(command, shell, exec) != 0)
	{
		close_fd(exec->fd);
		return (1);
	}
	return (0);
}

/*
** Orchestrates execution for a command that is part of a pipeline.
** 1. Creates a pipe if there is a next command.
** 2. Forks a new process.
** 3. Child process calls child_process_multiple_pipeline().
** Returns 0 on success (parent), 1 on failure (fork/pipe error).
*/
int	initialize_and_process_multiple_child(t_exec *exec,
	t_shell *shell, t_cmd *command)
{
	if (command->next)
	{
		if (pipe(exec->fd->fd) == -1)
		{
			perror("minishell: pipe");
			return (1);
		}
	}
	setup_execution_signals();
	exec->pid = fork();
	if (exec->pid < 0)
	{
		perror("fork failed in multiple child process");
		close(exec->fd->fd[0]);
		close(exec->fd->fd[1]);
		setup_parent_signals();
		return (1);
	}
	if (exec->pid == 0)
	{
		return (child_process_multiple_pipeline(exec, shell, command));
	}
	return (0);
}
