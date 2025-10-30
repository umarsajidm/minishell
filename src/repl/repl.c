/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:02:03 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/28 19:22:50 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Main REPL loop
 * - Reads input
 * - Tokenizes using arena
 * - Expands variables (disabled for now)
 * - Parses commands using arena (disabled for now)
 * - Executes commands (disabled for now)
 * - Loops until shell->running == false
 */
void repl_loop(t_shell *shell, t_arena **arena)
{
	char    *input;     // input line (arena memory)
	t_list  *tokens;    // token list (arena nodes)
	t_cmd   *commands;  // parsed commands (arena nodes)

	while (shell->running)
	{
		input = read_input(arena);               // read user input into arena
		if (!input)
			break;                               // Ctrl-D exits

		tokens = tokenize(input, arena);         // tokenize input using arena
		dbg_print_tokens(tokens);                // debug tokens

		/* variable expansion (disabled until implemented) */
		// expand_variables(tokens, shell);

		/* parse tokens into commands (disabled until implemented) */
		// commands = parse_tokens(tokens, arena);
		// dbg_print_cmds(commands);

		/* execute commands (disabled until implemented) */
		// execute_command(shell, commands);

		dbg_print_exit_code(shell->exit_code);   // debug exit code

		/* avoid unused variable warnings while features are stubbed */
		(void)tokens;
		(void)commands;
	}
}
