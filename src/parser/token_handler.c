/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:16:24 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 17:11:31 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Handle adding an expanded word to the command
 * - Deals with empty strings and field splitting
 * - Returns 1 on success, 0 on failure
 */
int	handle_empty_or_split_word(char *expanded, t_parser_state *p)
{
	if (expanded[0] == '\0' && p->tok->token[0] != '\''
		&& p->tok->token[0] != '"')
	{
		if (ft_strcmp(p->tok->token, "$") != 0)
		{
			if (!*p->cur && !ensure_current_cmd(p->cur, p->head, p->arena))
				return (0);
			return (1);
		}
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
 * Handle a word token
 * - Expands the token string
 * - Calls helper to handle empty words or field splitting
 */
int	handle_word_token(t_parser_state *p)
{
	char	*expanded;

	expanded = expand_string(p->tok->token, p->shell, p->arena);
	if (!expanded)
		return (0);
	return (handle_empty_or_split_word(expanded, p));
}

/* 
 * Handle a pipe token during parsing
 * - Checks for invalid pipe placement
 * - Ends current command by setting p->cur to NULL
 * - Returns 1 on success, -1 on syntax error
 */
int	handle_pipe_token(t_parser_state *p)
{
	if (!*p->cur || !(*p->cur)->argv || !(*p->cur)->argv[0])
		return (-1);
	*p->cur = NULL;
	return (1);
}

/* 
 * Handle an operator token
 * - Calls the appropriate handler for pipe or redirection tokens
 * - Returns -1 on syntax error, 0 if unknown operator, 1 on success
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
 * Add a word to the command's argv array
 * - Allocates new argv array in arena and copies existing words
 * - Returns 1 on success, 0 on failure
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
