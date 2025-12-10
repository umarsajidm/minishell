#include "minishell.h"

/* Create a new command node and initialize fields */
t_cmd	*create_cmd_node(t_arena **arena)
{
	t_cmd	*cmd;

	cmd = arena_alloc(arena, sizeof(t_cmd));      // allocate memory for command
	if (!cmd)
		return (NULL);                            // allocation failed
	cmd->argv = NULL;                              // initialize argv
	cmd->redirs = NULL;                            // initialize redirections
	cmd->next = NULL;                              // initialize next pointer
	cmd->unexpanded_cmd = NULL;
	return (cmd);                                  // return new command node
}

/* Append a new command node to the end of the command list */
t_cmd	*append_new_cmd(t_cmd **head, t_arena **arena)
{
	t_cmd	*node;
	t_cmd	*iter;

	node = create_cmd_node(arena);                 // allocate new command node
	if (!node)
		return (NULL);                             // allocation failed
	if (!*head)                                    // first command in list
	{
		*head = node;                              // set head
		return (*head);                             // return head
	}
	iter = *head;                                  // iterate to last node
	while (iter->next)
		iter = iter->next;
	iter->next = node;                             // append new node
	return (*head);                                // return head
}

/* Add a word to the command's argv array */
int	add_word_to_argv(t_cmd *cmd, const char *word, t_arena **arena)
{
	char	**new_argv;
	int		i;
	int		count;
	size_t	len;

	if (!cmd || !word)
		return (0);                                 // invalid input
	count = count_argv(cmd->argv);                 // count existing argv entries
	new_argv = arena_alloc(arena, sizeof(char *) * (count + 2)); // allocate new argv
	if (!new_argv)
		return (0);                                 // allocation failed
	i = 0;
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];               // copy existing pointers
		i++;
	}
	len = ft_strlen(word);                         // get length of new word
	new_argv[i] = arena_alloc(arena, len + 1);    // allocate space for word
	if (!new_argv[i])
		return (0);                                 // allocation failed
	ft_memcpy(new_argv[i], word, len + 1);        // copy word into argv
	new_argv[i + 1] = NULL;                        // null-terminate argv
	cmd->argv = new_argv;                           // update command argv
	return (1);                                     // success
}

/* Check if token is a pipe '|' */
int	is_pipe_token(const char *token_str)
{
	if (!token_str)
		return (0);                             // NULL token
	if (!ft_strcmp(token_str, "|"))
		return (1);                             // pipe token
	return (0);                                 // not a pipe
}

/* Check if token is a redirection '<', '>', '>>', '<<' */
int	is_redir_token(const char *token_str)
{
	if (!token_str)
		return (0);                             // NULL token
	if (!ft_strcmp(token_str, "<"))
		return (1);                             // input redirection
	if (!ft_strcmp(token_str, ">"))
		return (1);                             // output redirection
	if (!ft_strcmp(token_str, ">>"))
		return (1);                             // append redirection
	if (!ft_strcmp(token_str, "<<"))
		return (1);                             // heredoc
	return (0);                                 // not a redirection
}
