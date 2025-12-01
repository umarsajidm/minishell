#include "minishell.h"

static int	cd_av_error(char *oldpwd);
static int	cd_error(char *oldpwd);
static int	update_env(char *oldpwd, t_shell *shell);

int	ft_cd(t_cmd *cmds, t_shell *shell)
{
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (!cmds->argv[1] || ft_strncmp(cmds->argv[1], "--", 3) == 0)
	{
		if (!find_env_node("HOME", shell->env))
			return (cd_error(oldpwd));
		if (chdir((find_env_node("HOME", shell->env)->value)))
			return (cd_error(oldpwd));
		return (0);
	}
	if (cmds->argv[2])
		return (cd_av_error(oldpwd));
	if (chdir(cmds->argv[1]))
		return (cd_error(oldpwd));
	update_env(oldpwd, shell);
	return (0);
}

static int	update_env(char *oldpwd, t_shell *shell)
{
	char	*newpwd;
	t_env	*oldpwd_node;
	t_env	*pwd_node;

	newpwd = getcwd(NULL, 0);
	oldpwd_node = find_env_node("OLDPWD", shell->env);
	pwd_node = find_env_node("PWD", shell->env);
	if (oldpwd)
		if (oldpwd_node)
			update_cd_node(oldpwd, oldpwd_node);
	if (newpwd)
		if (pwd_node)
			update_cd_node(newpwd, pwd_node);
	return (0);
}

static int	cd_error(char *oldpwd)
{
	ft_putstr_fd("minishell: cd: ", 2);
	perror("");
	if (oldpwd)
		free(oldpwd);
	return (1);
}

static int	cd_av_error(char *oldpwd)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd("too many arguments\n", 2);
	if (oldpwd)
		free(oldpwd);
	return (1);
}
