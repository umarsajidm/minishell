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
