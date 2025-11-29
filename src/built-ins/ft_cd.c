#include "minishell.h"

int	ft_cd(t_cmd *cmds, t_shell *shell)
{
	(void)cmds;
	(void)shell;
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	printf("%s\n", oldpwd);
	free(oldpwd);
	return (0);
}