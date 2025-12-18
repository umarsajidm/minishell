#include "minishell.h"

static void	unset_node(t_shell *shell, t_env *match);
static void	free_env_node(t_env *node);

int	ft_unset(t_cmd *cmd, t_shell *shell)
{
	int		i;
	t_env	*match;

	if (!cmd->argv[1])
		return (0);
	i = 1;
	while (cmd->argv[i])
	{
		match = find_env_node(cmd->argv[i], shell->env);
		if (match)
			unset_node(shell, match);
		i++;
	}
	return (0);
}

t_env	*find_env_node(const char *str, t_env *head)
{
	char	*sign;
	size_t	len;

	sign = ft_strchr(str, '=');
	if (!sign)
		len = ft_strlen(str);
	else
		len = sign - str;
	while (head)
	{
		if (ft_strlen(head->key) == len && ft_strncmp(str, head->key, len) == 0)
			return (head);
		head = head->next;
	}
	return (NULL);
}

static void	unset_node(t_shell *shell, t_env *match)
{
	t_env	*prev;
	t_env	*current;

	if (!shell || !shell->env || !match)
		return ;
	if (shell->env == match)
		return (shell->env = shell->env->next, free_env_node(match));
	prev = shell->env;
	current = shell->env->next;
	while (current)
	{
		if (current == match)
		{
			prev->next = current->next;
			return (free_env_node(match));
		}
		prev = current;
		current = current->next;
	}
}

static void	free_env_node(t_env *node)
{
	if (node->key)
		free(node->key);
	if (node->value)
		free(node->value);
	free(node);
	node = NULL;
}
