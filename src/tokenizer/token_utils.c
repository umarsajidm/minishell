#include "minishell.h"

/*
 * Check if character is an operator
 */
int	is_operator_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);                             // operator found
	return (0);                                 // not an operator
}

/*
 * Skip whitespace characters
 */
int	skip_spaces(char *s, int i)
{
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;                                    // advance index
	return (i);                                 // return new index
}

/*
 * Duplicate a substring from start to end
 */
char	*dup_word(t_arena **arena, const char *str, int start, int end)
{
	char	*word;
	int		len;

	len = end - start;
	word = arena_alloc(arena, len + 1);         // alloc string
	if (!word)
		return (NULL);                          // alloc failed
	ft_memcpy(word, str + start, len);          // copy chars
	word[len] = '\0';                           // null terminate
	return (word);
}

/*
 * Helper: Append node to end of list
 */
static void	list_append(t_list **tokens, t_list *node)
{
	t_list	*tmp;

	if (!*tokens)
		*tokens = node;                         // set head
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;                    // find tail
		tmp->next = node;                       // link node
	}
}

/*
 * Allocate token struct, node, and add to list
 * - Updated to use t_token_type
 */
int	create_token_node(t_arena **ar, t_list **toks, char *content, t_token_type type)
{
	t_token	*tok;
	t_list	*node;

	tok = arena_alloc(ar, sizeof(t_token));     // alloc struct
	if (!tok)
		return (0);                             // alloc failed
	tok->token = content;                       // set string
	tok->type = type;                           // set type
	tok->next = NULL;                           // (optional) init unused next
	node = arena_alloc(ar, sizeof(t_list));     // alloc node
	if (!node)
		return (0);                             // alloc failed
	node->content = tok;                        // attach content
	node->next = NULL;
	list_append(toks, node);                    // add to list
	return (1);                                 // success
}
