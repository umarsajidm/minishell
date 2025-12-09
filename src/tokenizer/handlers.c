#include "minishell.h"

/*
 * Handle | < > << >>
 */
int	handle_operator(char *s, int i, t_list **tokens, t_arena **arena)
{
	int		len;
	char	*op_str;

	len = 1;                                    // default length
	if ((s[i] == '<' && s[i + 1] == '<')
		|| (s[i] == '>' && s[i + 1] == '>'))
		len = 2;                                // double char op
	op_str = dup_word(arena, s, i, i + len);    // alloc string
	if (!op_str)
		return (-1);                            // alloc failed
	if (!create_token_node(arena, tokens, op_str, T_OPERATOR))
		return (-1);                            // node creation failed
	return (i + len);                           // return next index
}

/*
 * Handle ' and "
 */
int	handle_quote(char *s, int i, t_list **tokens, t_arena **arena)
{
	int		start;
	char	*sub;

	start = i;                                  // save start
	i++;                                        // skip opening quote
	while (s[i] && s[i] != s[start])
		i++;                                    // find closing quote
	if (!s[i])
	{
		ft_printf("minishell: syntax error: unmatched quote\n");
		return (-1);
	}
	i++;                                        // include closing quote
	sub = dup_word(arena, s, start, i);         // alloc string
	if (!sub || !create_token_node(arena, tokens, sub, T_QUOTE))
		return (-1);                            // alloc or node failed
	return (i);                                 // return next index
}

/*
 * Handle normal words
 */
int	handle_word(char *s, int i, t_list **tokens, t_arena **arena)
{
	int		start;
	char	*sub;

	start = i;                                  // save start
	while (s[i] && !is_operator_char(s[i]) && s[i] != ' '
		&& s[i] != '\t' && s[i] != '\'' && s[i] != '"'
		&& s[i] != '\n')
		i++;                                    // find end of word
	if (i == start)
		return (i);                             // empty word
	sub = dup_word(arena, s, start, i);         // alloc string
	if (!sub)
		return (-1);                            // alloc failed
	if (!create_token_node(arena, tokens, sub, T_WORD))
		return (-1);                            // node creation failed
	return (i);                                 // return next index
}
