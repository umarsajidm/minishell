#include "execution.h"

void waitstatus(pid_t pid,  t_shell *shell);

static void init_fd(t_fd	*fd)
{
	fd->fd[0] = -1;
	fd->fd[1] = -1;
	fd->prev_fd = -1;
	fd->in_fd = -1;
	fd->out_fd = -1;
}

void close_fd(t_fd *fd)
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
}

static void parent_loop(t_cmd *cmd, t_fd *fd)
{
	//parent closes the prev
	if (fd->prev_fd != -1)
            close(fd->prev_fd);

        // Keep current read end for next command
    if (cmd->next != NULL)
        fd->prev_fd = fd->fd[0];
    else
        fd->prev_fd = -1;
        // Close write end in parent
    close(fd->fd[1]);
}

int	execute_pipe(t_cmd *cmd, t_shell *shell, t_fd *fd, char **arr)
{
	if (cmd->redirs)
		applying_redir(cmd->redirs, &(fd->in_fd), &(fd->out_fd));
	if (fd->prev_fd != -1)
		dup2(fd->prev_fd, STDIN_FILENO);
	if (fd->in_fd != -1)
		dup2(fd->in_fd, STDIN_FILENO);
	if (cmd->next != NULL && fd->out_fd == -1)
		dup2(fd->fd[1], STDOUT_FILENO);
	if (fd->out_fd != -1)
		dup2(fd->out_fd, STDOUT_FILENO);
	close_fd(fd);
	if (execution(cmd, shell, arr) == 1)
	{

		return (1);
	}
	return(0);
}

void execution_pipeline(t_cmd *command, t_shell *shell)
{
    pid_t pid;
    t_fd fd;

    char **envp = envp_arr(shell);
    init_fd(&fd);
	if (!command->next)
	{
		if (is_builtin(command) && is_parent_level_builtin(command))
		{
			shell->exit_code = run_builtin(command, shell, &shell->arena);
			close_fd(&fd);
			freearray(envp);
			return ;
		}
		else if (!command->next)
		{
			if (child_process(command, shell, &fd, envp))
			{
				close_fd(&fd);
				freearray(envp);
				return ;
			}
		}
		close_fd(&fd);
		freearray(envp);
		return ;
	}
	while (command)
	{
		pipe(fd.fd);
		pid = fork();
		if (pid < 0)
			perror("fork failed in child process");
		if (pid == 0)
		{
			if (execute_pipe(command, shell, &fd, envp) == 1)
			{
				// freearray(envp);
				close_fd(&fd);
				return ;
			}
		}
		parent_loop(command, &fd);
		command = command->next;
	}
	close_fd(&fd);
	waitstatus(pid, shell);
    freearray(envp);
	return ;
}

void waitstatus(pid_t pid,  t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if WIFEXITED(status)
		shell->exit_code = WEXITSTATUS(status);
}
