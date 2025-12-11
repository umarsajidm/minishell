#include "minishell.h"

/* 
 * Process one token based on current character
 * - Calls the proper handler: operator, quote, or word
 * - Updates index `i` to next character
 * - Allocates everything inside arena
 * Returns 1 on success, 0 on error (allocation failure or unmatched quote)
 */
int	process_token(char *input, int *i, t_list **tokens, t_arena **arena)
{
	int	ret;                                        // handler return (next index)

	ret = -1;                                       // defensive init

	if (is_operator_char(input[*i]))                // operator | < >
		ret = handle_operator(input, *i, tokens, arena);
	else                                            // normal word
		ret = handle_word(input, *i, tokens, arena);

	if (ret == -1)                                  // handler failed (alloc or unmatched quote)
		return (0);
	*i = ret;                                       // update index
	return (1);                                     // success
}

/* 
 * Tokenize the input string into a linked list of tokens
 * - Handles operators, quotes, and normal words
 * - Skips spaces and tabs
 * - Allocates all tokens and nodes inside arena
 * Returns pointer to token list or NULL if input is NULL or allocation fails
 */
t_list	*tokenize(char *input, t_arena **arena)
{
	t_list	*tokens;                                // head of token list
	int		i;                                      // input index

	if (!input)
		return (NULL);                              // nothing to tokenize

	tokens = NULL;                                  // start with empty list
	i = 0;                                          // starting index
	while (input[i])
	{
		i = skip_spaces(input, i);                 // skip whitespace
		if (!input[i])
			break;                                  // end of string
		if (!process_token(input, &i, &tokens, arena)) // process token
			return (NULL);                          // error occurred (unmatched quote or alloc)
	}
	return (tokens);                                // return token list
}
