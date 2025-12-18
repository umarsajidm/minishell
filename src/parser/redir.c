/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 15:55:50 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 17:02:06 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Determine the type of redirection based on token string
 * - "<"  => R_INPUT
 * - ">"  => R_OUTPUT
 * - ">>" => R_APPEND
 * - any other (default) => R_HEREDOC
 */
static t_redir_type	get_redir_type(const char *token_str)
{
	if (!ft_strcmp(token_str, "<"))
		return (R_INPUT);
	if (!ft_strcmp(token_str, ">"))
		return (R_OUTPUT);
	if (!ft_strcmp(token_str, ">>"))
		return (R_APPEND);
	return (R_HEREDOC);
}

/* 
 * Get the target of a redirection
 * - For heredoc, call handle_heredoc and store contents in cur->heredoc
 * - Otherwise, expand the target string using shell expansion
 * - Returns the target string allocated in arena, or NULL on failure
 */
static char	*get_redir_target(t_redir_type type, t_token *next_tok,
								t_shell *shell, t_cmd *cur)
{
	char	*target;

	if (type == R_HEREDOC)
	{
		cur->heredoc = handle_heredoc(cur, shell, next_tok->token);
		if (!cur->heredoc || g_signal)
			return (NULL);
		target = arena_strdup(&shell->arena, next_tok->token);
	}
	else
		target = expand_string(next_tok->token, shell, &shell->arena);
	return (target);
}

/* 
 * Create a new t_redir structure with the given type and target
 * - Allocates memory in arena
 * - Returns NULL on allocation failure
 */
static t_redir	*create_redir(t_redir_type type, const char *target,
	t_arena **arena)
{
	t_redir	*redir;
	size_t	tlen;

	redir = arena_alloc(arena, sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	tlen = ft_strlen(target);
	redir->target = arena_alloc(arena, tlen + 1);
	if (!redir->target)
		return (NULL);
	ft_memcpy(redir->target, target, tlen + 1);
	redir->next = NULL;
	return (redir);
}

/* 
 * Add a t_redir node to the end of cmd->redirs linked list
 * - Allocates the node via create_redir
 * - Returns 1 on success, 0 on failure
 */
int	add_redirection(t_cmd *cmd, t_redir_type type, const char *target,
	t_arena **arena)
{
	t_redir	*redir;
	t_redir	*iter;

	if (!cmd || !target)
		return (0);
	redir = create_redir(type, target, arena);
	if (!redir)
		return (0);
	if (!cmd->redirs)
		cmd->redirs = redir;
	else
	{
		iter = cmd->redirs;
		while (iter->next)
			iter = iter->next;
		iter->next = redir;
	}
	return (1);
}

/* 
 * Handle a redirection token in the parser
 * - tokens_ref points to the current token in the list
 * - Ensures current command exists, gets redirection type & target
 * - Adds the redirection to the command
 * - Advances tokens_ref to skip the target token
 * - Returns 1 on success, 0 on allocation failure, -1 on syntax error
 */
int	handle_redir_token(t_list **tokens_ref, t_cmd **cur,
						t_cmd **head, t_shell *shell)
{
	t_list			*next_node;
	t_token			*tok;
	t_token			*next_tok;
	t_redir_type	type;
	char			*target;

	tok = (*tokens_ref)->content;
	next_node = (*tokens_ref)->next;
	if (!next_node)
		return (-1);
	next_tok = next_node->content;
	if (!next_tok || !next_tok->token)
		return (-1);
	if (!ensure_current_cmd(cur, head, &shell->arena))
		return (0);
	type = get_redir_type(tok->token);
	target = get_redir_target(type, next_tok, shell, *cur);
	if (!target)
		return (0);
	if (!add_redirection(*cur, type, target, &shell->arena))
		return (0);
	*tokens_ref = next_node;
	return (1);
}
