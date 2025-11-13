#include "execution.h"

/* 

==1==we will check whether it is a builtin or binary
==2==separate pipelines for both
==3==if not builtin then child process
==4==before creating the child process we will see if we have pipe or redirections
==5==also need to handle a case where binary and builtinn both commands come

buitin commands
echo
pwd
cd
env
export
unset
exit

1- builtin --no fork
2- redirection + echo or bin  -- fork
3- pipe + redirection + builtin --fork

int dup2(int oldfd, int newfd);
  dup2()
       The dup2() system call performs the same task as dup(), but instead of using the lowest-numbered  unused  file
       descriptor,  it uses the file descriptor number specified in newfd.  In other words, the file descriptor newfd
       is adjusted so that it now refers to the same open file description as oldfd.

       If the file descriptor newfd was previously open, it is closed before being reused;  the  close  is  performed
       silently (i.e., any errors during the close are not reported by dup2()).

       The  steps  of closing and reusing the file descriptor newfd are performed atomically.  This is important, be‐
       cause trying to implement equivalent functionality using close(2) and dup() would be subject  to  race  condi‐
       tions,  whereby newfd might be reused between the two steps.  Such reuse could happen because the main program
       is interrupted by a signal handler that allocates a file descriptor, or because a parallel thread allocates  a
       file descriptor.

       Note the following points:

       •  If oldfd is not a valid file descriptor, then the call fails, and newfd is not closed.

       •  If  oldfd  is a valid file descriptor, and newfd has the same value as oldfd, then dup2() does nothing, and
          returns newfd.
	RETURN VALUE
       On  success, these system calls return the new file descriptor.  On error, -1 is returned, and errno is set to
       indicate the error.
*/

static void pipe_execution(t_cmd	*command, t_shell *shell);

//static void pipeline(t_cmd	*command, t_shell *shell);

void	execution(char **cmd, char **env);

static int	is_builtin(char *cmd)
{
	//just a prototype , have to discuss with jonathen if we can move up with it
	// need to ponder over, whether we can apply redirections here or not
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	return (0); //need to think about this statement
}

void execution_pipeline(t_cmd *command, t_shell *shell)
{
   
	t_cmd	*t_command;
	int		i;
	int		pipe_number;
	
	i = 0;
	t_command = command;
    while(t_command)
	{
		if (t_command->argv)
			i++;
		t_command = t_command->next;
	}
	pipe_number = i - 1;
	if (!pipe_number && !is_builtin(command->argv[0]))//if there is no pipe and cmd is binary
		child_process(command, shell);
	else	
		pipe_execution(command, shell);
}
static void pipe_execution(t_cmd	*command, t_shell *shell)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	t_cmd	*t_command;
	int		i;
	int		pipe_number;
	
	i = 0;
	t_command = command;
    while(t_command)
	{
		if (t_command->argv)
			i++;
		t_command = t_command->next;
	}
	pipe_number = i - 1;

	fd[0] = -1;
	fd[1] = -1;
	int fdsec[2];
	fdsec[0] = -1;
	fdsec[1] = -1;
	i = 0;
	//first command: cmd->argv[0] = command name and else is arg.
	//changing the stdout (that is terminal) to the writing end of pipe
	char **envp = envp_arr(shell);
	pipe(fd);
	while(pipe_number > 0)
	{
		pid1 = fork();
		if (pid1 == 0)
		{
			if (i == 0)
				dup2(fd[1], STDOUT_FILENO);
			else if (i > 0)
			{
				dup2(fdsec[0], STDIN_FILENO);
				close(fdsec[0]);
				close(fdsec[1]);
			}
			
			close(fd[0]);
			close(fd[1]);
			execution(command->argv, envp);
		}
		pid2 = fork();
		if (pid2 == 0)
		{
			command = command->next;
			dup2(fd[0], STDIN_FILENO);
			close(fd[0]);
			close(fd[1]);
			if (pipe_number > 1)
			{
				pipe(fdsec);
				dup2(fdsec[0], STDOUT_FILENO);
				close(fdsec[0]);
				close(fdsec[1]);
				i++;
			}
			execution(command->argv, envp);
		}
		pipe_number--;
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}


// static void pipeline(t_cmd	*command, t_shell *shell)
// {
// 	if (!is_builtin(command->argv[0]))
// 		child_process(command, shell);
// }

// static void init_fd(int fd[2])
// {
// 	fd[0] = -1;
// 	fd[1] = -1;
// }

// static void close_fd(int fd[2])
// {
// 	close(&fd[0]);
// 	close(&fd[1]);
// }

// static void pipe_execution(t_cmd *command, t_shell *shell)
// {
// 	int		fd[2];
// 	pid_t	pid1;
// 	pid_t	pid2;

// 	fd[0] = -1;
// 	fd[1] = -1;
// 	//first command: cmd->argv[0] = command name and else is arg.
// 	//changing the stdout (that is terminal) to the writing end of pipe
// 	char **envp = envp_arr(shell);
// 	pipe(fd);
	
// 	while (command)
// 	{
// 		pid1 = fork();
// 		if (pid1 == 0)
// 		{
// 			dup2(fd[1], STDOUT_FILENO);
// 			close(fd[0]);
// 			close(fd[1]);
// 			execution(command->argv, envp);
// 		}
// 		pid2 = fork();
// 		if (pid2 == 0)
// 		{
// 			command = command->next;
// 			dup2(fd[0], STDIN_FILENO);
// 			if (command->next != NULL)
// 				dup2(fd[1], STDOUT_FILENO);
// 			close(fd[0]);
// 			close(fd[1]);
// 			execution(command->argv, envp);
// 		}
		
// 	}

// 	close(fd[0]);
// 	close(fd[1]);
// 	waitpid(pid1, NULL, 0);
// 	waitpid(pid2, NULL, 0);
// }

/*
what is status in waitpid
what is WIFEXITED, WIFSIGNALED, WIFEXITSTATUS, WTERMSIG
*/