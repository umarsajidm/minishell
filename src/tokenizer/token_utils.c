#include "minishell.h"

/* 
 * Handle operators in the input string
 * - Supports |, <, >, <<, >>
 * - Adds operator as token to list using arena
 * Returns next index after operator or -1 on error
 */
int	handle_operator(char *s, int i, t_list **tokens, t_arena **arena)
{
	int		len;                                   // operator length
	t_list	*node;                                 // new list node
	t_token	*tok;                                  // token struct
	t_list	*tmp;                                  // iterator

	len = 1;                                       // default operator length
	if ((s[i] == '<' && s[i + 1] == '<')
		|| (s[i] == '>' && s[i + 1] == '>'))
		len = 2;                                   // double operator

	tok = arena_alloc(arena, sizeof(t_token));     // allocate token struct
	if (!tok)
		return (-1);                              // alloc failed

	tok->token = arena_alloc(arena, len + 1);      // allocate operator string
	if (!tok->token)
		return (-1);                              // alloc failed

	ft_memcpy(tok->token, s + i, len);             // copy operator chars
	tok->token[len] = '\0';                        // null-terminate
	tok->type = T_OPERATOR;                        // set type

	node = arena_alloc(arena, sizeof(t_list));     // allocate list node
	if (!node)
		return (-1);                              // alloc failed
	node->content = tok;                           // attach token
	node->next = NULL;                             // end of list

	if (!*tokens)                                  // first token
		*tokens = node;
	else
	{
		tmp = *tokens;                             // append at end
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;                          // link new node
	}
	return (i + len);                              // next index
}

/* 
 * Handle quoted strings in the input
 * - Supports single (') and double (") quotes
 * - Everything inside quotes is a single token
 * Returns next index after closing quote or -1 on error
 */
int	handle_quote(char *s, int i, t_list **tokens, t_arena **arena)
{
	t_list	*node;                                  // new node
	t_token	*tok;                                   // token struct
	char	quote;                                  // which quote
	int		start;                                  // start index
	t_list	*tmp;                                   // iterator

	quote = s[i];                                   // opening quote
	i++;                                            // move past quote
	start = i;                                      // content starts here

	while (s[i] && s[i] != quote)
		i++;                                        // find closing
	if (!s[i])
	{
		ft_printf("minishell: syntax error: unmatched quote\n"); // error msg
		return (-1);                                // unmatched quote
	}

	tok = arena_alloc(arena, sizeof(t_token));      // alloc token
	if (!tok)
		return (-1);                              // alloc failed

	tok->token = arena_alloc(arena, i - start + 1); // alloc quoted string
	if (!tok->token)
		return (-1);                              // alloc failed

	ft_memcpy(tok->token, s + start, i - start);    // copy inner content
	tok->token[i - start] = '\0';                   // null-terminate
	tok->type = T_QUOTE;                            // set type

	node = arena_alloc(arena, sizeof(t_list));      // alloc list node
	if (!node)
		return (-1);                              // alloc failed
	node->content = tok;                            // set content
	node->next = NULL;                              // last node

	if (!*tokens)                                   // first token
		*tokens = node;
	else
	{
		tmp = *tokens;                             // append at end
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;                          // link new node
	}
	return (i + 1);                                 // index after closing quote
}

/* 
 * Handle normal words in the input
 * - Word ends at space, tab, operator, or quote
 * Returns next index after word or -1 on error
 */
int	handle_word(char *s, int i, t_list **tokens, t_arena **arena)
{
	t_list	*node;                                  // new node
	t_token	*tok;                                   // token struct
	int		start;                                  // start index
	t_list	*tmp;                                   // iterator

	start = i;                                      // beginning of word
	while (s[i] && !is_operator_char(s[i]) && s[i] != ' '
		&& s[i] != '\t' && s[i] != '\'' && s[i] != '"'
		&& s[i] != '\n')
		i++;                                        // find end of word
	if (i == start)
		return (i);                                // empty, nothing to store

	tok = arena_alloc(arena, sizeof(t_token));      // allocate token struct
	if (!tok)
		return (-1);                              // alloc failed

	tok->token = arena_alloc(arena, i - start + 1); // allocate word string
	if (!tok->token)
		return (-1);                              // alloc failed

	ft_memcpy(tok->token, s + start, i - start);    // copy word
	tok->token[i - start] = '\0';                   // null-terminate
	tok->type = T_WORD;                             // set type

	node = arena_alloc(arena, sizeof(t_list));      // allocate list node
	if (!node)
		return (-1);                              // alloc failed
	node->content = tok;                            // set content
	node->next = NULL;                              // last node

	if (!*tokens)                                   // first token
		*tokens = node;
	else
	{
		tmp = *tokens;                             // append to end
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = node;                          // link node
	}
	return (i);                                     // next index
}
