#include "minishell.h"

void	update_cd_node(char *value, t_env *node)
{
	if (node->value)
		free(node->value);
	node->value = ft_strdup(value);
	free(value);
}
