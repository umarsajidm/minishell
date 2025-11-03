#include "minishell.h"

/* 
 * Handle operators in the input string
 * - Supports |, <, >, <<, >>
 * - Adds operator as token to list using arena
 * Returns next index after operator or -1 on error
 */
int	handle_operator(char *s, int i, t_list **tokens, t_arena **arena)
{
	int		len;                                   // operator length (1 or 2)
	t_list	*node;                                 // new list node
	t_token *tok;                                  // token struct
	t_list	*tmp;                                  // iterator

	len = 1;                                        // default length
	if ((s[i] == '<' && s[i + 1] == '<') || (s[i] == '>' && s[i + 1] == '>'))
		len = 2;                                   // double operator

	tok = arena_alloc(arena, sizeof(t_token));    // allocate token struct
	if (!tok)
		return (-1);                              // allocation failed

	tok->str = arena_alloc(arena, len + 1);       // allocate string
	if (!tok->str)
		return (-1);                              // allocation failed

	ft_memcpy(tok->str, s + i, len);             // copy operator
	tok->str[len] = '\0';                         // null-terminate
	tok->type = T_OPERATOR;                       // set type

	node = arena_alloc(arena, sizeof(t_list));    // allocate list node
	if (!node)
		return (-1);                              // allocation failed
	node->content = tok;                           // set content
	node->next = NULL;                             // terminate node

	if (!*tokens)                                  // start with empty list
		*tokens = node;
	else
	{
		tmp = *tokens;                             // append to end
		while (tmp->next)
			tmp = tmp->next;                       // move to last node
		tmp->next = node;                           // link new node
	}

	return (i + len);                              // return next index
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
	t_token *tok;                                   // token struct
	char	quote;                                  // quote char
	int		start;                                  // start index
	t_list	*tmp;                                   // iterator

	quote = s[i];                                   // current quote
	i++;                                            // move past quote
	start = i;                                      // start of content

	while (s[i] && s[i] != quote)
		i++;                                        // find closing quote
	if (!s[i])
	{
		ft_printf("minishell: syntax error: unmatched quote\n"); // error
		return (-1);
	}

	tok = arena_alloc(arena, sizeof(t_token));      // allocate token struct
	if (!tok)
		return (-1);

	tok->str = arena_alloc(arena, i - start + 1);  // allocate string
	if (!tok->str)
		return (-1);
	ft_memcpy(tok->str, s + start, i - start);     // copy content
	tok->str[i - start] = '\0';                    // null-terminate
	tok->type = T_QUOTE;                           // set type

	node = arena_alloc(arena, sizeof(t_list));     // allocate list node
	if (!node)
		return (-1);
	node->content = tok;                            // set content
	node->next = NULL;                              // terminate node

	if (!*tokens)                                  // start with empty list
		*tokens = node;
	else
	{
		tmp = *tokens;                             // append to end
		while (tmp->next)
			tmp = tmp->next;                       // move to last node
		tmp->next = node;                           // link new node
	}

	return (i + 1);                                // index after closing quote
}

/* 
 * Handle normal words in the input
 * - Word ends at space, tab, operator, or quote
 * Returns next index after word or -1 on error
 */
int	handle_word(char *s, int i, t_list **tokens, t_arena **arena)
{
	t_list	*node;                                  // new node
	t_token *tok;                                   // token struct
	int		start;                                  // start index
	t_list	*tmp;                                   // iterator

	start = i;                                      // start of word
	while (s[i] && !is_operator_char(s[i]) && s[i] != ' ' && s[i] != '\t'
		&& s[i] != '\'' && s[i] != '"' && s[i] != '\n')
		i++;                                        // find end of word
	if (i == start)
		return (i);                                // no word

	tok = arena_alloc(arena, sizeof(t_token));      // allocate token struct
	if (!tok)
		return (-1);

	tok->str = arena_alloc(arena, i - start + 1);  // allocate string
	if (!tok->str)
		return (-1);
	ft_memcpy(tok->str, s + start, i - start);     // copy content
	tok->str[i - start] = '\0';                    // null-terminate
	tok->type = T_WORD;                            // set type

	node = arena_alloc(arena, sizeof(t_list));     // allocate list node
	if (!node)
		return (-1);
	node->content = tok;                            // set content
	node->next = NULL;                              // terminate node

	if (!*tokens)                                  // start with empty list
		*tokens = node;
	else
	{
		tmp = *tokens;                             // append to end
		while (tmp->next)
			tmp = tmp->next;                       // move to last node
		tmp->next = node;                           // link new node
	}
	return (i);                                    // next index
}
