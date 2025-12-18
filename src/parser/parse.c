/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 16:16:12 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 20:59:34 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Process a word or quoted token
 * - Delegates to word handler
 * - Prints allocation error on failure
/* 
 * Process a word token
 * - Calls handle_word_token to expand and add the word
 * - Returns -1 on allocation failure, 0 on success
>
 */
static int	process_word_token(t_parser_state *p)
{
	if (!handle_word_token(p))
	{
		ft_printf("minishell: parse error: alloc fail\n");
		return (-1);
	}
	return (0);
}


static int	process_operator_token(t_parser_state *p)
{
	int	res;

	res = handle_operator_token(p);
	if (res == -1)
	{
		ft_printf("minishell: syntax error near '%s'\n", p->tok->token);
		return (-1);
	}
	if (res == 0)
	{
		if (!g_signal)
			ft_printf("minishell: parse error: alloc fail\n");
		return (-1);
	}
	return (0);
}

/*
 * Dispatch token processing based on token type
 */
static int	process_token(t_parser_state *p)
{
	if (!p->iterator || !*p->iterator || !(*p->iterator)->content)
	{
		ft_printf("minishell: invalid token\n");
		return (-1);
	}
	p->tok = (*p->iterator)->content;
	if (p->tok->type == T_WORD || p->tok->type == T_QUOTE)
		return (process_word_token(p));
	else
		return (process_operator_token(p));
}

/*
 * Helper: Iterates through tokens and processes them.
 * - Handles error restoration internally to keep logic consistent.
 * - Returns 0 on failure, 1 on success.
 */
static int	run_parser_loop(t_parser_state *p, int saved_stdin)
{
	while (*(p->iterator))
	{
		if (process_token(p) == -1)
		{
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdin);
			return (0);
		}
		*(p->iterator) = (*(p->iterator))->next;
	}
	return (1);
}

/*
 * Parse token list into a command list
 * - Restores STDIN on failure
 */
t_cmd	*parse_tokens(t_list *tokens, t_shell *shell, t_arena **arena)
{
	t_parser_state	p;
	int				saved_stdin;
	t_cmd			*head;
	t_cmd			*cur;
	t_list			*it;

	head = NULL;
	cur = NULL;
	p.cur = &cur;
	p.head = &head;
	p.shell = shell;
	p.arena = arena;
	p.iterator = &it;
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		return (NULL);
	if (!run_parser_loop(&p, saved_stdin))
		return (NULL);
	close(saved_stdin);
	return (head);
}
