/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jtarvain <jtarvain@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 13:10:09 by jtarvain          #+#    #+#             */
/*   Updated: 2025/12/18 13:10:11 by jtarvain         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	print_export_vars(t_env *head);
static int	valid_syntax(const char *str);
static void	print_export_error(char *str);
static int	add_to_env(const char *str, t_shell *shell);

/*
** Executes the 'export' built-in command.
** 1. If no arguments, prints environment variables sorted alphabetically.
** 2. Iterates through arguments to export new variables.
** 3. Validates syntax for each argument (must start with letter/_ and contain only alphanum/_).
** 4. Adds or updates variables in the environment.
** Returns 0 on success, 1 on error (syntax or malloc).
*/
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

/*
** Prints exported environment variables sorted by key.
** 1. Allocates an array of pointers to environment nodes.
** 2. Sorts the array alphabetically.
** 3. Prints the sorted variables in specific format (declare -x KEY="VALUE").
** 4. Frees the temporary array.
** Returns 0 on success, 1 on allocation failure.
*/
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

/*
** Prints a syntax error message for invalid export identifiers.
*/
static void	print_export_error(char *str)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": not a valid indentifier\n", 2);
}

/*
** Checks if a variable name has valid syntax.
** - Must start with a letter or underscore.
** - Can only contain alphanumeric characters or underscores up to the '='.
** Returns 1 if valid, 0 otherwise.
*/
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

/*
** Adds or updates an environment variable.
** 1. Checks if the variable already exists.
** 2. Updates it if found.
** 3. Adds a new node if not found.
** Returns 1 on success, 0 on failure (malloc).
*/
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
