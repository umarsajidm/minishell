#include "minishell.h"

void	add_argv_to_cmd(t_cmd *cmd, char **argv, t_arena **arena)
{
	int	i;

	i = 0;
	while (argv[i])
	{
		add_word_to_argv(cmd, argv[i], arena);
		i++;
	}
}

t_cmd	*create_cmd_node(t_arena **arena)
{
	t_cmd	*cmd;

	cmd = arena_alloc(arena, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->unexpanded_cmd = NULL;
	return (cmd);
}

t_cmd	*append_new_cmd(t_cmd **head, t_arena **arena)
{
	t_cmd	*node;
	t_cmd	*iter;

	node = create_cmd_node(arena);
	if (!node)
		return (NULL);
	if (!*head)
	{
		*head = node;
		return (*head);
	}
	iter = *head;
	while (iter->next)
		iter = iter->next;
	iter->next = node;
	return (*head);
}

int	add_word_to_argv(t_cmd *cmd, const char *word, t_arena **arena)
{
	char	**new_argv;
	int		i;
	int		count;
	size_t	len;

	if (!cmd || !word)
		return (0);
	count = count_argv(cmd->argv);
	new_argv = arena_alloc(arena, sizeof(char *) * (count + 2));
	if (!new_argv)
		return (0);
	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	len = ft_strlen(word);
	new_argv[i] = arena_alloc(arena, len + 1);
	if (!new_argv[i])
		return (0);
	ft_memcpy(new_argv[i], word, len + 1);
	new_argv[i + 1] = NULL;
	cmd->argv = new_argv;
	return (1);
}

int	is_pipe_token(const char *token_str)
{
	if (!token_str)
		return (0);
	if (!ft_strcmp(token_str, "|"))
		return (1);
	return (0);
}

int	is_redir_token(const char *token_str)
{
	if (!token_str)
		return (0);
	if (!ft_strcmp(token_str, "<"))
		return (1);
	if (!ft_strcmp(token_str, ">"))
		return (1);
	if (!ft_strcmp(token_str, ">>"))
		return (1);
	if (!ft_strcmp(token_str, "<<"))
		return (1);
	return (0);
}