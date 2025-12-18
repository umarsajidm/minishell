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

void	init_fd(t_fd	*fd)
{
	fd->fd[0] = -1;
	fd->fd[1] = -1;
	fd->prev_fd = -1;
	fd->in_fd = -1;
	fd->out_fd = -1;
}

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

void	parent_loop(t_cmd *cmd, t_fd *fd)
{
	if (fd->prev_fd != -1)
		close(fd->prev_fd);
	if (cmd->next != NULL)
		fd->prev_fd = fd->fd[0];
	else
		fd->prev_fd = -1;
	close(fd->fd[1]);
}

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
	if (execution(command, shell, exec) == 1)
		exit_after_execve(shell, exec);
	free(exec->path_to_exec);
	if (exec->envp != NULL)
		freearray(exec->envp);
	execution_cleanup(shell);
	exit(shell->exit_code);
}
