#include "minishell.h"

/* 
 * Check if a character is a shell operator
 * - Returns 1 if the character is '|', '<', or '>', otherwise 0
 */
int	is_operator_char(char c)
{
	if (c == '|' || c == '<' || c == '>')
		return (1);                                 // operator found
	return (0);                                     // not an operator
}

/* 
 * Duplicate a substring from `start` to `end` (exclusive)
 * - Allocates memory from the arena
 * - Returns pointer to duplicated string, or NULL on failure
 */
char	*dup_word(t_arena **arena, const char *str, int start, int end)
{
	char	*word;                                   // result buffer
	int		i;                                       // loop index
	int		len;                                     // number of chars

	if (!arena || !*arena)
		return (NULL);                              // invalid arena ptr

	len = end - start;
	if (len < 0)
		return (NULL);                              // invalid range

	word = arena_alloc(arena, len + 1);             // allocate substring
	if (!word)
		return (NULL);                              // alloc failed

	i = 0;
	while (start < end)
	{
		word[i] = str[start];                       // copy char
		i++;
		start++;
	}
	word[i] = '\0';                                 // null-terminate
	return (word);                                  // return allocated substring
}

/* 
 * Skip spaces and tabs starting from index `i`
 * - Returns index of first non-space/tab character
 */
int	skip_spaces(char *s, int i)
{
	while (s[i] && (s[i] == ' ' || s[i] == '\t'))
		i++;                                        // continue skipping
	return (i);                                     // first non-space index
}
