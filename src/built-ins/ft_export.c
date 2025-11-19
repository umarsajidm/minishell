#include "minishell.h"

static int	print_export_vars(t_env *head);
static int	valid_syntax(const char *str);
static void	print_export_error(const char *str);

int	ft_export(t_cmd *cmds, t_shell *shell)
{
	int	i;
	int	ret;

	if (!cmds->argv[1])
		return (print_export_vars(shell->env));
	ret = 0;
	i = 1;
	while (cmds->argv[i])
	{
		if (!valid_syntax(cmds->argv[i]))
		{
			print_export_error(cmds->argv[i]);
			ret = 1;
		}
		else
			add_to_env(cmds->argv[i], shell);
		i++;
	}
	return (ret);
}

static int	print_export_vars(t_env *head)
{

}

static void	print_export_error(const char *str)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": not a valid indentifier", 2);
}

static int	valid_syntax(const char *str)
{
	int	i;
	
	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}