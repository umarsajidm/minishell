/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 20:15:46 by achowdhu          #+#    #+#             */
/*   Updated: 2025/12/18 21:12:05 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
 * Handle single and double quote characters during expansion
 * - Updates quote state (none, single, double)
 * - Appends quote characters when inside opposite quote type
 */
static void	handle_quotes(const char *str, t_state *state)
{
	if (str[state->i] == '\'')
	{
		if (state->quote == 2)
			state->res = append_char(state->res, &state->len,
					'\'', state->arena);
		else if (state->quote == 1)
			state->quote = 0;
		else
			state->quote = 1;
	}
	else if (str[state->i] == '"')
	{
		if (state->quote == 1)
			state->res = append_char(state->res, &state->len,
					'"', state->arena);
		else if (state->quote == 2)
			state->quote = 0;
		else
			state->quote = 2;
	}
	state->i++;
}

/*
 * Handle environment variable expansion after `$`
 * - Expands variable value
 * - Replaces unquoted spaces with FIELD_SEP
 */
static void	handle_dollar(const char *str, t_state *state)
{
	char	*tmp;
	size_t	j;

	state->i++;
	tmp = expand_variable(str, &state->i, state->shell, state->arena);
	if (!tmp)
		return ;
	j = 0;
	while (tmp[j])
	{
		if (ft_isspace(tmp[j]) && state->quote == 0)
			state->res = append_char(state->res, &state->len, FIELD_SEP,
					state->arena);
		else
			state->res = append_char(state->res, &state->len, tmp[j],
					state->arena);
		j++;
	}
}

/*
 * Process a single character during expansion
 * - Dispatches to quote, dollar, or literal handlers
 */
static void	process_char(const char *str, t_state *state)
{
	if (str[state->i] == '\'' || str[state->i] == '"')
		handle_quotes(str, state);
	else if (str[state->i] == '$' && state->quote != 1)
		handle_dollar(str, state);
	else
	{
		state->res = append_char(state->res, &state->len, str[state->i],
				state->arena);
		state->i++;
	}
}

/*
 * Expand variables and quotes in a string
 * - Handles quotes, `$` expansion, and field splitting markers
 * - Returns expanded string allocated in arena
 */
char	*expand_string(const char *str, t_shell *shell, t_arena **arena)
{
	t_state	state;

	if (!str)
		return (NULL);
	state.res = arena_strdup(arena, "");
	if (!state.res)
		return (NULL);
	state.len = 0;
	state.i = 0;
	state.quote = 0;
	state.shell = shell;
	state.arena = arena;
	if (!str)
		return (NULL);
	while (str[state.i])
	{
		process_char(str, &state);
		if (!state.res)
			return (NULL);
	}
	return (state.res);
}

/*
 * Expand all argv entries of a command
 * - Replaces each argument with its expanded version
 */
int	expand_command_argv(t_cmd *cmd, t_shell *shell, t_arena **arena)
{
	int		i;
	char	*expanded;

	if (!cmd || !cmd->argv)
		return (1);
	i = 0;
	while (cmd->argv[i])
	{
		expanded = expand_string(cmd->argv[i], shell, arena);
		if (!expanded)
			return (0);
		cmd->argv[i] = expanded;
		i++;
	}
	return (1);
}
