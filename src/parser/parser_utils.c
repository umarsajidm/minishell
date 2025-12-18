/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:16:12 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 21:11:31 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Allocate and initialize a new command node
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
 * Ensure a current command exists
 * - Creates and links a new command if needed
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
 * Check if token represents a pipe operator
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
 * Check if token represents a redirection operator
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
