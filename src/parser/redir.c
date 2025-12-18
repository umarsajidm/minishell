/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD
/*   Created: 2025/12/18 16:16:12 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 20:50:16 by achowdhu         ###   ########.fr       */
=======
/*   Created: 2025/12/18 15:55:50 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 17:02:06 by achowdhu         ###   ########.fr       */
>>>>>>> main
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
 * Resolve the target of a redirection
 * - Handles heredoc separately
 * - Expands target when needed
 */
char	*get_redir_target(t_redir_type type, t_token *next_tok,
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
 * Create a redirection node
 */
static t_redir	*create_redirection_node(t_redir_type type, const char *target,
	t_arena **arena)
{
	t_redir	*redir;
	size_t	tlen;

	if (!target)
		return (NULL);
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
 * Add a redirection node to the command
 */
int	add_redirection(t_cmd *cmd, t_redir_type type, const char *target,
	t_arena **arena)
{
	t_redir	*redir;
	t_redir	*iter;

	if (!cmd)
		return (0);
	redir = create_redirection_node(type, target, arena);
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
 * Handle a redirection token during parsing
 * - Validates next token
 * - Attaches redirection to current command
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
	if (!next_tok || !next_tok->token || next_tok->type == T_OPERATOR)
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
