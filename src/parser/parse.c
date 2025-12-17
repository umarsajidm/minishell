#include "minishell.h"

int	handle_word_token(t_cmd **cur, t_cmd **head, t_token *tok,
					t_shell *shell, t_arena **arena)
{
	char	*expanded;
	bool	add_arg;
	char	**words;
	int		i;

	add_arg = true;
	expanded = expand_string(tok->token, shell, arena);
	if (!expanded)
		return (0);
	if (expanded[0] == '\0' && tok->token[0] != '\'' && tok->token[0] != '"')
	{
		if (ft_strcmp(tok->token, "$") != 0)
			add_arg = false;
	}
	if (!add_arg)
	{
		if (!*cur && !ensure_current_cmd(cur, head, arena))
			return (0);
		return (1);
	}
	if (ft_strchr(expanded, FIELD_SEP))
	{
		words = field_split(expanded, arena);
		if (!words)
			return (0);
		i = 0;
		while (words[i])
		{
			if (!ensure_current_cmd(cur, head, arena)
				|| !add_word_to_argv(*cur, words[i], arena))
				return (0);
			if (!(*cur)->argv && i == 0)
				(*cur)->unexpanded_cmd = tok->token;
			i++;
		}
		return (1);
	}
	if (!ensure_current_cmd(cur, head, arena))
		return (0);
	if (!(*cur)->argv)
		(*cur)->unexpanded_cmd = tok->token;
	if (!add_word_to_argv(*cur, expanded, arena))
		return (0);
	return (1);
}


/* Handle pipe token */
int handle_pipe_token(t_token *tok, t_cmd **cur)
{
    (void)tok;
    if (!*cur || !(*cur)->argv || !(*cur)->argv[0]) // invalid pipe
        return (-1);
    *cur = NULL;
    return (1);
}

/* Handle operators including pipes and redirections */
int handle_operator_token(t_list **tokens_ref, t_cmd **cur,
    t_cmd **head, t_shell *shell, t_arena **arena)
{
    t_token *tok;

    tok = (*tokens_ref)->content;
    if (is_pipe_token(tok->token))
        return (handle_pipe_token(tok, cur));
    if (is_redir_token(tok->token))
	{
		setup_hd_signals();
        return (handle_redir_token(tokens_ref, cur, head, shell, arena));
	}
    return (0);
}

/* Process a single token */
static int  process_token(t_list **iterator, t_cmd **cur, t_cmd **head,
    t_shell *shell, t_arena **arena)
{
    int     res;
    t_token *tok;

    if (!iterator || !*iterator || !(*iterator)->content)
    {
        ft_printf("minishell: invalid token\n");
        return (-1);
    }
    tok = (*iterator)->content;

    if (tok->type == T_WORD || tok->type == T_QUOTE)
    {
        if (!handle_word_token(cur, head, tok, shell, arena))
        {
            ft_printf("minishell: parse error: alloc fail\n");
            return (-1);
        }
    }
    else
    {
        res = handle_operator_token(iterator, cur, head, shell, arena);
        if (res == -1)
        {
            ft_printf("minishell: syntax error near '%s'\n", tok->token);
            return (-1);
        }
        if (res == 0)
        {
            ft_printf("minishell: parse error: alloc fail\n");
            return (-1);
        }
    }
    return (0);
}

/* Parse token list into command list */
t_cmd   *parse_tokens(t_list *tokens, t_shell *shell, t_arena **arena)
{
    t_cmd   *head;
    t_cmd   *cur;
    t_list  *it;

    head = NULL;
    cur = NULL;
    it = tokens;
    while (it)
    {
        if (process_token(&it, &cur, &head, shell, arena) == -1)
            return (NULL);
        it = it->next;
    }
    return (head);
}