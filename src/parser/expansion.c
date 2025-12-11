#include "minishell.h"

/* CASE 1 & 2: Quotes logic */
static void	handle_quotes(const char *str, t_state *state)
{
	if (str[state->i] == '\'')
	{
		if (state->quote == 2) // inside double quotes
			state->res = append_char(state->res, &state->len, str[state->i],
					state->arena);
		else if (state->quote == 1) // toggle single quote state
			state->quote = 0;
		else
			state->quote = 1;
	}
	else if (str[state->i] == '"')
	{
		if (state->quote == 1) // inside single quotes
			state->res = append_char(state->res, &state->len, str[state->i],
					state->arena);
		else if (state->quote == 2) // toggle double quote state
			state->quote = 0;
		else
			state->quote = 2;
	}
	state->i++; // skip the quote itself
}

/* CASE 3: Variable ($) - Only if NOT in single quotes */
static void	handle_dollar(const char *str, t_state *state)
{
	char	*tmp;
	size_t	j;

	state->i++; // skip '$' sign
	tmp = expand_variable(str, &state->i, state->shell, state->arena);
	if (!tmp)
		return ; // alloc fail
	j = 0;
	while (tmp[j])
	{
		state->res = append_char(state->res, &state->len, tmp[j],
				state->arena);
		j++;
	}
}

/* * Dispatcher to split main loop logic */
static void	process_char(const char *str, t_state *state)
{
	if (str[state->i] == '\'' || str[state->i] == '"')
		handle_quotes(str, state);
	else if (str[state->i] == '$' && state->quote != 1)
		handle_dollar(str, state);
	else
	{
		/* CASE 4: Normal Character */
		state->res = append_char(state->res, &state->len, str[state->i],
				state->arena);
		state->i++; // next char
	}
}

/* * Main Expansion Logic */
char	*expand_string(const char *str, t_shell *shell, t_arena **arena)
{
	t_state	state;

	/* FIX: Initialize to empty string so "" returns valid pointer, not NULL */
	state.res = arena_strdup(arena, "");
	if (!state.res)
		return (NULL);
	state.len = 0;
	state.i = 0;
	state.quote = 0; // 0=None, 1=Single, 2=Double
	state.shell = shell;
	state.arena = arena;
	if (!str)
		return (NULL);
	while (str[state.i])
	{
		process_char(str, &state);
		if (!state.res)
			return (NULL); // Real alloc fail
	}

	return (state.res);
}

/* * Expand all arguments in the command list */
int	expand_command_argv(t_cmd *cmd, t_shell *shell, t_arena **arena)
{
	int		i;
	char	*expanded;

	if (!cmd || !cmd->argv)
		return (1); // nothing to do
	i = 0;
	while (cmd->argv[i])
	{
		expanded = expand_string(cmd->argv[i], shell, arena);
		if (!expanded)
			return (0); // expansion error
		cmd->argv[i] = expanded; // update argv
		i++;
	}
	return (1); // success
}
