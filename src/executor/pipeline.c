#include "execution.h"

// typedef enum e_redir_type
// {
// 	R_INPUT,
// 	R_OUTPUT,
// 	R_APPEND,
// 	R_HEREDOC
// }	t_redir_type;

// typedef struct s_redir
// {
// 	t_redir_type	type;   /* type of redirection */
// 	char			*target;/* file or heredoc target */
// 	struct s_redir	*next;  /* next redirection */
// }	t_redir;

// typedef struct s_cmd
// {
// 	char			**argv;  /* argument vector */
// 	t_redir			*redirs; /* redirections */
// 	struct s_cmd	*next;   /* next command */
// }	t_cmd;

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


*/
static void pipeline(t_cmd	*command, t_shell *shell);

static int	is_builtin(char *cmd)
{
	//just a prototype , have to discuss with jonathen if we can move up with it
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
	if (!pipe_number)
		pipeline(command, shell);
}
void pipe_execution(t_cmd	*command)
{
	int	fd[2]
}


static void pipeline(t_cmd	*command, t_shell *shell)
{
	if (!is_builtin(command->argv[0]))
		child_process(command, shell);
}