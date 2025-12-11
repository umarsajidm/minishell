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
 * Handle words, including those with quotes
 */
int	handle_word(char *s, int i, t_list **tokens, t_arena **arena)
{
	int		start;
	char	*sub;
	char	quote;

	start = i;
	quote = 0;
	while (s[i])
	{
		if (quote)
		{
			if (s[i] == quote)
				quote = 0;
		}
		else
		{
			if (s[i] == '\'' || s[i] == '"')
				quote = s[i];
			else if (is_operator_char(s[i]) || s[i] == ' ' || s[i] == '\t'
				|| s[i] == '\n')
				break ;
		}
		i++;
	}
	if (quote)
	{
		ft_printf("minishell: syntax error: unmatched quote\n");
		return (-1);
	}
	sub = dup_word(arena, s, start, i);
	if (!sub || !create_token_node(arena, tokens, sub, T_WORD))
		return (-1);
	return (i);
}