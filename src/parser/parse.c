#include "minishell.h"

/* Handle word or quoted token */
int	handle_word_token(t_cmd **cur, t_cmd **head, t_token *tok,
	t_shell *shell, t_arena **arena)
{
	char	*to_add;
	char	*expanded;

	/* ensure a current command node exists (will append to head if needed) */
	if (!ensure_current_cmd(cur, head, arena))
		return (0);                                /* allocation failed */

	if (tok->type == T_QUOTE)
		to_add = tok->token;                        /* quoted token */
	else
	{
		expanded = expand_string(tok->token, shell, arena);
		if (!expanded)
			return (0);                            /* allocation failed */
		to_add = expanded;                         /* expanded string */
	}
	if (!add_word_to_argv(*cur, to_add, arena))
		return (0);                                /* allocation failed */
	return (1);                                    /* success */
}

/* Handle pipe token */
int	handle_pipe_token(t_token *tok, t_cmd **cur)
{
	(void)tok;
	if (!*cur || !(*cur)->argv || !(*cur)->argv[0]) // invalid pipe
		return (-1);                                 // syntax error
	*cur = NULL;                                     // end current command
	return (1);                                      // success
}

/* Handle operators including pipes and redirections */
int	handle_operator_token(t_list **tokens_ref, t_cmd **cur,
	t_cmd **head, t_arena **arena)
{
	t_token	*tok;

	tok = (*tokens_ref)->content;                   // get current token
	if (is_pipe_token(tok->token))                 // pipe
		return (handle_pipe_token(tok, cur));
	if (is_redir_token(tok->token))               // redirection
		return (handle_redir_token(tokens_ref, cur, head, arena));
	return (0);                                     // unknown operator
}

/* 
 * Process a single token
 * - Handles words, quoted strings, and operators
 * - Returns 0 on success, -1 on syntax or allocation error
 */
static int	process_token(t_token *tok, t_cmd **cur, t_cmd **head,
	t_shell *shell, t_arena **arena)
{
	int		res;
	t_list	temp_node;
	t_list	*temp_list;

	if (!tok)
	{
		ft_printf("minishell: invalid token\n"); // token is NULL
		return (-1);
	}

	if (tok->type == T_WORD || tok->type == T_QUOTE)
	{
		// Handle word or quoted token
		if (!handle_word_token(cur, head, tok, shell, arena))
		{
			ft_printf("minishell: parse error: alloc fail\n");
			return (-1);
		}
	}
	else
	{
		// Wrap token in temporary list for operator handling
		temp_node.content = tok;                   // store token
		temp_node.next = NULL;                     // no next node
		temp_list = &temp_node;                    // pointer to temp_node

		// Handle pipe or redirection
		res = handle_operator_token(&temp_list, cur, head, arena);
		if (res == -1)
		{
			ft_printf("minishell: syntax error near '%s'\n", tok->token);
			return (-1);
		}
		if (res == 0)
		{
			ft_printf("minishell: parse error: alloc fail\n");
			return (-1);
		}
	}
	return (0);                                     // success
}

/* Parse token list into command list */
t_cmd	*parse_tokens(t_list *tokens, t_shell *shell, t_arena **arena)
{
	t_cmd	*head;
	t_cmd	*cur;
	t_list	*it;

	head = NULL;                                   // initialize command list head
	cur = NULL;                                    // current command pointer
	it = tokens;
	while (it)
	{
		if (process_token(it->content, &cur, &head, shell, arena) == -1)
			return (NULL);                         // error during token processing
		it = it->next;                              // move to next token
	}
	return (head);                                 // return command list
}
