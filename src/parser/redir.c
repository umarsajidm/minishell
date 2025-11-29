#include "minishell.h"

/* Ensure there is a current command; create if NULL */
static int	ensure_current_cmd(t_cmd **cur, t_cmd **head, t_arena **arena)
{
	t_cmd	*new_cmd;
	t_cmd	*iter;

	if (*cur)
		return (1);                              // current command exists
	new_cmd = create_cmd_node(arena);           // allocate new command
	if (!new_cmd)
		return (0);                              // allocation failed
	if (!*head)
		*head = new_cmd;                         // set as head
	else
	{
		iter = *head;                           // append to end
		while (iter->next)
			iter = iter->next;
		iter->next = new_cmd;                   // link new command
	}
	*cur = new_cmd;                             // update current command
	return (1);                                  // success
}

/* Get redirection type enum from token string */
t_redir_type	get_redir_type(const char *token_str)
{
	if (!ft_strcmp(token_str, "<"))
		return (R_INPUT);                        // input redirection
	if (!ft_strcmp(token_str, ">"))
		return (R_OUTPUT);                       // output redirection
	if (!ft_strcmp(token_str, ">>"))
		return (R_APPEND);                       // append redirection
	return (R_HEREDOC);                          // heredoc redirection
}

/* Add a redirection to the command's redir list */
int	add_redirection(t_cmd *cmd, t_redir_type type, const char *target,
	t_arena **arena)
{
	t_redir	*redir;
	t_redir	*iter;
	size_t	tlen;

	if (!cmd || !target)
		return (0);                                 // invalid input
	redir = arena_alloc(arena, sizeof(t_redir));   // allocate redirection
	if (!redir)
		return (0);                                 // allocation failed
	redir->type = type;                            // set redirection type
	tlen = ft_strlen(target);                       // get target length
	redir->target = arena_alloc(arena, tlen + 1);  // allocate target string
	if (!redir->target)
		return (0);                                 // allocation failed
	ft_memcpy(redir->target, target, tlen + 1);    // copy target string
	redir->next = NULL;                             // initialize next pointer
	if (!cmd->redirs)
		cmd->redirs = redir;                         // first redirection
	else
	{
		iter = cmd->redirs;                          // append to end
		while (iter->next)
			iter = iter->next;
		iter->next = redir;
	}
	return (1);                                     // success
}

/* Handle a redirection token and its target */
int	handle_redir_token(t_list **tokens_ref, t_cmd **cur,
	t_cmd **head, t_arena **arena)
{
	t_list		*next_node;
	t_token		*tok;
	t_token		*next_tok;
	t_redir_type	type;
	char		*heredoc_content;

	tok = (*tokens_ref)->content;                // current token
	next_node = (*tokens_ref)->next;             // next token node
	if (!next_node)
		return (-1);                             // missing target
	next_tok = next_node->content;               // next token
	if (!next_tok || !next_tok->token)
		return (-1);                             // invalid target
	if (!ensure_current_cmd(cur, head, arena))
		return (0);                              // failed to create command

	type = get_redir_type(tok->token);           // determine redir type
	if (type == R_HEREDOC)
	{
		heredoc_content = handle_heredoc(*cur, arena, next_tok->token);
		if (!heredoc_content)
			return (0);                          // failed to read heredoc
		(*cur)->heredoc = heredoc_content;       // store heredoc content
	}
	if (!add_redirection(*cur, type, next_tok->token, arena))
		return (0);                              // failed to add redirection
	*tokens_ref = next_node;                     // move token pointer
	return (1);                                  // success
}
