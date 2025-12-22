/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 19:04:13 by musajid           #+#    #+#             */
/*   Updated: 2025/12/17 19:54:54 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Initializes the file descriptor structure.
** Sets all FD values to -1 to indicate they are closed/unused.
*/
void	init_fd(t_fd	*fd)
{
	fd->fd[0] = -1;
	fd->fd[1] = -1;
	fd->prev_fd = -1;
	fd->in_fd = -1;
	fd->out_fd = -1;
}

/*
** Closes all valid file descriptors in the structure.
** Checks each FD before closing to avoid errors.
** Resets the structure after closing.
*/
void	close_fd(t_fd *fd)
{
	if (fd->fd[0] >= 0)
		close(fd->fd[0]);
	if (fd->fd[1] >= 0)
		close(fd->fd[1]);
	if (fd->prev_fd >= 0)
		close(fd->prev_fd);
	if (fd->in_fd >= 0)
		close(fd->in_fd);
	if (fd->out_fd >= 0)
		close(fd->out_fd);
	init_fd(fd);
}

/*
** Manages pipe file descriptors in the parent process loop.
** Closes the previous read end (if any).
** If there's a next command, saves the current pipe's read end for it.
** Closes the current pipe's write end.
*/
void	parent_loop(t_cmd *cmd, t_fd *fd)
{
	if (fd->prev_fd != -1)
		close(fd->prev_fd);
	if (cmd->next != NULL)
		fd->prev_fd = fd->fd[0];
	else
	{
		close(fd->fd[0]);
		fd->prev_fd = -1;
	}
	close(fd->fd[1]);
}

/*
** Sets up file descriptors for the child process and executes command.
** 1. Redirects input from file or previous pipe.
** 2. Redirects output to file or next pipe.
** 3. Closes internal FD structure.
** 4. Executes the command.
** 5. Cleans up memory and exits if execution fails or finishes.
*/
int	fds_manipulation_and_execution(t_cmd *command,
	t_shell *shell, t_exec *exec)
{
	if (exec->fd->in_fd != -1)
		dup2(exec->fd->in_fd, STDIN_FILENO);
	else if (exec->fd->prev_fd != -1)
		dup2(exec->fd->prev_fd, STDIN_FILENO);
	if (exec->fd->out_fd != -1)
		dup2(exec->fd->out_fd, STDOUT_FILENO);
	else if (command->next != NULL)
		dup2(exec->fd->fd[1], STDOUT_FILENO);
	close_fd(exec->fd);
	if (command->argv && command->argv[0])
	{
		if (execution(command, shell, exec) == 1)
			exit_after_execve(shell, exec);
	}
	free(exec->path_to_exec);
	if (exec->envp != NULL)
		freearray(exec->envp);
	execution_cleanup(shell);
	exit(shell->exit_code);
}
