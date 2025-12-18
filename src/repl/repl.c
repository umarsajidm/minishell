/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 14:30:24 by musajid           #+#    #+#             */
/*   Updated: 2025/12/18 22:38:27 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	process_line(t_shell *shell, t_arena **arena, char *input)
{
	t_list	*tokens;
	t_cmd	*commands;

	if (is_blank_line(input))
		return ;
	tokens = tokenize(input, arena);
	if (!tokens)
	{
		shell->exit_code = 2;
		return ;
	}
	commands = parse_tokens(tokens, shell, arena);
	if (!commands)
	{
		setup_parent_signals();
		if (g_signal != 0)
			return ;
		else
			shell->exit_code = 2;
		return ;
	}
	if (commands && (commands->argv || commands->unexpanded_cmd))
		main_pipeline(shell, commands);
}

static int	handle_input_and_history(t_shell *shell, char *input)
{
	if (!input)
	{
		ft_exit2(NULL, shell);
		return (0);
	}
	if (g_signal != 0)
	{
		shell->exit_code = 128 + g_signal;
		g_signal = 0;
	}
	if (input && *input)
		add_history(input);
	else if (!input && shell->exit_flow == FLOW_EXIT)
	{
		if (shell->should_print_exit_message && isatty(STDIN_FILENO))
			ft_putstr_fd("exit\n", STDOUT_FILENO);
		shell->should_print_exit_message = false;
		shell->running = false;
		shell->exit_flow = FLOW_OK;
		return (0);
	}
	return (1);
}

void	repl_loop(t_shell *shell, t_arena **arena)
{
	char	*input;

	while (shell->running)
	{
		input = read_input(arena);
		if (!handle_input_and_history(shell, input))
			break ;
		if (input)
			process_line(shell, arena, input);
		if (g_signal)
			g_signal = 0;
		arena_clear(arena);
		if (shell->exit_flow == FLOW_EXIT)
		{
			if (shell->should_print_exit_message && isatty(STDIN_FILENO))
				ft_putstr_fd("exit\n", STDOUT_FILENO);
			shell->should_print_exit_message = false;
			shell->running = false;
			shell->exit_flow = FLOW_OK;
		}
	}
}

void	non_interactive_loop(t_shell *shell, t_arena **arena)
{
	char	*input;

	while (shell->running)
	{
		input = read_input(arena);
		if (!input)
			break ;
		if (input)
			process_line(shell, arena, input);
		arena_clear(arena);
		if (shell->exit_flow == FLOW_EXIT)
		{
			if (shell->should_print_exit_message)
				ft_putstr_fd("exit\n", STDOUT_FILENO);
			shell->should_print_exit_message = false;
			shell->running = false;
			shell->exit_flow = FLOW_OK;
		}
	}
}

char	*handle_heredoc(t_cmd *cmd, t_shell *shell, const char *delimiter)
{
	(void)cmd;
	if (!delimiter)
		return (NULL);
	return (read_heredoc(shell, delimiter));
}
