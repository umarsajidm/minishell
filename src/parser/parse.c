/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 15:53:56 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 16:59:52 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Process a word token
 * - Calls handle_word_token to expand and add the word
 * - Returns -1 on allocation failure, 0 on success
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

/* 
 * Process an operator token
 * - Calls handle_operator_token to handle pipe or redirection
 * - Returns -1 on syntax error or allocation failure
 * - Returns 0 if the operator is unknown
 */
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
 * Process a single token from the iterator
 * - Determines if token is a word/quote or operator
 * - Calls the appropriate processing function
 * - Returns -1 on invalid token or processing error, 0 on success
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
 * Process all tokens in the list
 * - Iterates through the tokens and calls process_token
 * - Returns 1 on success, 0 on parse error
 * - Restores STDIN on error
 */
static int	process_all_tokens(t_parser_state *p, t_list *tokens,
				int saved_stdin)
{
	t_list	*it;

	it = tokens;
	while (it)
	{
		if (process_token(p) == -1)
		{
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdin);
			return (0);
		}
		it = it->next;
	}
	return (1);
}

/* 
 * Parse a list of tokens into a linked list of t_cmd
 * - Allocates commands in the arena
 * - Returns pointer to head of t_cmd list or NULL on failure
 */
t_cmd	*parse_tokens(t_list *tokens, t_shell *shell, t_arena **arena)
{
	t_parser_state	p;
	int				saved_stdin;
	t_cmd			*head;
	t_cmd			*cur;

	head = NULL;
	cur = NULL;
	p.cur = &cur;
	p.head = &head;
	p.shell = shell;
	p.arena = arena;
	p.iterator = &tokens;
	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		return (NULL);
	if (!process_all_tokens(&p, tokens, saved_stdin))
		return (NULL);
	close(saved_stdin);
	return (head);
}
