/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 15:30:50 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 17:02:48 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Create a new t_cmd node
 * - Allocates memory for t_cmd using the arena
 * - Initializes all fields to NULL
 * - Returns pointer to the new node or NULL on allocation failure
 */
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

/* 
 * Ensure there is a current command in the linked list
 * - If *cur is NULL, allocates a new t_cmd node and appends it to *head
 * - Updates *cur to point to the current command
 * - Returns 1 on success, 0 on allocation failure
 */
int	ensure_current_cmd(t_cmd **cur, t_cmd **head, t_arena **arena)
{
	t_cmd	*new_cmd;
	t_cmd	*iter;

	if (*cur)
		return (1);
	new_cmd = create_cmd_node(arena);
	if (!new_cmd)
		return (0);
	if (!*head)
		*head = new_cmd;
	else
	{
		iter = *head;
		while (iter->next)
			iter = iter->next;
		iter->next = new_cmd;
	}
	*cur = new_cmd;
	return (1);
}

/* 
 * Check if a token string is a pipe operator "|"
 * - Returns 1 if true, 0 otherwise
 * - Returns 0 if token_str is NULL
 */
int	is_pipe_token(const char *token_str)
{
	if (!token_str)
		return (0);
	if (!ft_strcmp(token_str, "|"))
		return (1);
	return (0);
}

/* 
 * Check if a token string represents a redirection operator
 * - Supported operators: "<", ">", ">>", "<<"
 * - Returns 1 if token_str is a redirection, 0 otherwise
 * - Returns 0 if token_str is NULL
 */
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
