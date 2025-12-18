#include "minishell.h"

static int	handle_field_splitting(char *expanded, t_parser_state *p)
{
	char	**words;
	int		i;

	words = field_split(expanded, p->arena);
	if (!words)
		return (0);
	i = 0;
	while (words[i])
	{
		if (!ensure_current_cmd(p->cur, p->head, p->arena)
			|| !add_word_to_argv(*p->cur, words[i], p->arena))
			return (0);
		if (!(*p->cur)->argv && i == 0)
			(*p->cur)->unexpanded_cmd = p->tok->token;
		i++;
	}
	return (1);
}

static int	handle_word_token(t_parser_state *p)
{
	char	*expanded;
	bool	add_arg;

	add_arg = true;
	expanded = expand_string(p->tok->token, p->shell, p->arena);
	if (!expanded)
		return (0);
	if (expanded[0] == '\0' && p->tok->token[0] != '\''
		&& p->tok->token[0] != '"')
	{
		if (ft_strcmp(p->tok->token, "$") != 0)
			add_arg = false;
	}
	if (!add_arg)
	{
		if (!*p->cur && !ensure_current_cmd(p->cur, p->head, p->arena))
			return (0);
		return (1);
	}
	if (ft_strchr(expanded, FIELD_SEP))
		return (handle_field_splitting(expanded, p));
	if (!ensure_current_cmd(p->cur, p->head, p->arena))
		return (0);
	if (!(*p->cur)->argv)
		(*p->cur)->unexpanded_cmd = p->tok->token;
	if (!add_word_to_argv(*p->cur, expanded, p->arena))
		return (0);
	return (1);
}

static int	handle_pipe_token(t_parser_state *p)
{
	(void)p->tok;
	if (!*p->cur || !(*p->cur)->argv || !(*p->cur)->argv[0])
		return (-1);
	*p->cur = NULL;
	return (1);
}

static int	handle_operator_token(t_parser_state *p)
{
	if (is_pipe_token(p->tok->token))
		return (handle_pipe_token(p));
	if (is_redir_token(p->tok->token))
	{
		setup_hd_signals();
		return (handle_redir_token(p->iterator, p->cur, p->head, p->shell));
	}
	return (0);
}

static int	process_word_token(t_parser_state *p)
{
	if (!handle_word_token(p))
	{
		ft_printf("minishell: parse error: alloc fail\n");
		return (-1);
	}
	return (0);
}

static int	process_operator_token(t_parser_state *p)
{
	int		res;

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

t_cmd	*parse_tokens(t_list *tokens, t_shell *shell, t_arena **arena)
{
	t_parser_state	p;
	int			saved_stdin;
	t_cmd		*head;
	t_cmd		*cur;
	t_list		*it;

	head = NULL;
	cur = NULL;
	it = tokens;
	p.cur = &cur;
	p.head = &head;
	p.shell = shell;
	p.arena = arena;
	p.iterator = &it;
	saved_stdin = -1;
	saved_stdin = dup(STDIN_FILENO);

	while (it)
	{
		if (process_token(&p) == -1)
		{
			dup2(saved_stdin, STDIN_FILENO);
			close(saved_stdin);
			return (NULL);
		}
		it = it->next;
	}
	close(saved_stdin);
	return (head);
}
