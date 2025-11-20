#include "minishell.h"

static t_env	*alloc_node(const char *str);

int	update_env_node(const char *str, t_shell *shell)
{
	t_env	*match;
	char	*key;

	match = find_env_node(str, shell->env);
	key = ft_strchr(str, '=');
	key++;
	if (!*key)
		return (1); 
	if (match->value)
	{
		free(match->value);
		match->value = NULL;
	}
	match->value = ft_strdup(key);
	if (!match->value)
			return (1);
	return (0);
}

int	add_env_node(const char *str, t_shell *shell)
{
	if (!shell->env)
	{
		
	}
	while (shell->env->next)
		shell->env = shell->env->next;
	return (0);
}

static t_env	*alloc_node(const char *str)
{

}