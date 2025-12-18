#include "minishell.h"

int	process_token(char *input, int *i, t_list **tokens, t_arena **arena)
{
	int	ret;

	ret = -1;
	if (is_operator_char(input[*i]))
		ret = handle_operator(input, *i, tokens, arena);
	else
		ret = handle_word(input, *i, tokens, arena);
	if (ret == -1)
		return (0);
	*i = ret;
	return (1);
}

t_list	*tokenize(char *input, t_arena **arena)
{
	t_list	*tokens;
	int		i;

	if (!input)
		return (NULL);
	tokens = NULL;
	i = 0;
	while (input[i])
	{
		i = skip_spaces(input, i);
		if (!input[i])
			break ;
		if (!process_token(input, &i, &tokens, arena))
			return (NULL);
	}
	return (tokens);
}
