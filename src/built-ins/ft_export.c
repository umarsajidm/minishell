#include "minishell.h"

static int	print_export_vars(t_env *head);
static int	valid_syntax(const char *str);
static void	print_export_error(char *str);
static int	add_to_env(const char *str, t_shell *shell);

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
		{
			if (!add_to_env(cmds->argv[i], shell))
			{
				ft_putstr_fd("minishell: export: malloc fail\n", 2);
				ret = 1;
			}
		}
		i++;
	}
	return (ret);
}

static int	print_export_vars(t_env *head)
{
	t_env	**env_strings;
	t_env	*current;
	int		len;

	current = head;
	if (current == NULL)
		return (0);
	len = get_env_length(current);
	env_strings = alloc_assign(len, current);
	if (!env_strings)
	{
		ft_putstr_fd("minishell: export: malloc fail\n", 2);
		return (1);
	}
	sort_env(&env_strings);
	print_export(env_strings);
	free(env_strings);
	return (0);
}

static void	print_export_error(char *str)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": not a valid indentifier\n", 2);
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

static int	add_to_env(const char *str, t_shell *shell)
{
	if (!str || !*str)
		return (0);
	if (find_env_node(str, shell->env))
	{
		if (!update_env_node(str, shell))
			return (0);
	}
	else
	{
		if (!add_env_node(str, shell))
			return (0);
	}
	return (1);
}
