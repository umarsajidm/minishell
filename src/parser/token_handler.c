/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 19:38:14 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 21:11:20 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Decide if an empty argument should be added
 * - Returns true if argument should be added
 */
static bool	should_add_argument(char *expanded, char *original)
{
	if (expanded[0] == '\0' && original[0] != '\'' && original[0] != '"')
		if (ft_strcmp(original, "$") != 0)
			return (false);
	return (true);
}

/* 
 * Handle a word or quoted token
 * - Expands variables
 * - Handles empty results and field splitting
 * - Appends result to current command argv
 */
int	handle_word_token(t_parser_state *p)
{
	char	*expanded;
	bool	add_arg;

	expanded = expand_string(p->tok->token, p->shell, p->arena);
	if (!expanded)
		return (0);
	add_arg = should_add_argument(expanded, p->tok->token);
	if (!add_arg)
	{
		if (!*p->cur && !ensure_current_cmd(p->cur, p->head, p->arena))
			return (0);
		return (1);
	}
	if (ft_strchr(expanded, FIELD_SEP))
		return (handle_field_splitting(expanded, p));
	if (!ensure_current_cmd(p->cur, p->head, p->arena))
		return (0);
	if (!(*p->cur)->argv)
		(*p->cur)->unexpanded_cmd = p->tok->token;
	if (!add_word_to_argv(*p->cur, expanded, p->arena))
		return (0);
	return (1);
}

/*
 * Handle a pipe operator token
 * - Validates pipe placement
 * - Ends the current command
 */
int	handle_pipe_token(t_parser_state *p)
{
	(void)p->tok;
	if (!*p->cur || !(*p->cur)->argv || !(*p->cur)->argv[0])
		return (-1);
	*p->cur = NULL;
	return (1);
}

/*
 * Dispatch operator token handling
 * - Routes to pipe or redirection handler
 */
int	handle_operator_token(t_parser_state *p)
{
	if (is_pipe_token(p->tok->token))
		return (handle_pipe_token(p));
	if (is_redir_token(p->tok->token))
	{
		setup_hd_signals();
		return (handle_redir_token(p->iterator, p->cur, p->head, p->shell));
	}
	return (0);
}

/*
 * Append a word to a command argv array
 * - Reallocates argv in arena
 * - NULL-terminates argv
 */
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
