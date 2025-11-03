<<<<<<< HEAD
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musajid <musajid@hive.student.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 16:02:03 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/30 13:54:32 by musajid          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

=======
>>>>>>> main
#include "minishell.h"

/* Main REPL loop
 * - Reads input
 * - Tokenizes using arena
 * - Expands variables (disabled for now)
 * - Parses commands using arena
 * - Executes commands (disabled for now)
 * - Loops until shell->running == false
 */
<<<<<<< HEAD
 
void	repl_loop(t_shell *shell, char **env)
{
	char	*input;
	t_list	*tokens;
	t_list	*temp;
=======
void repl_loop(t_shell *shell, t_arena **arena)
{
	char    *input;     // input line (arena memory)
	t_list  *tokens;    // token list (arena nodes)
	t_cmd   *commands;  // parsed commands (arena nodes)
>>>>>>> main

	while (shell->running)
	{
		input = read_input(arena);               // read user input into arena
		if (!input)
			break;                               // Ctrl-D exits

		tokens = tokenize(input, arena);         // tokenize input using arena
		dbg_print_tokens(tokens);                // debug tokens

		/* variable expansion (disabled until implemented) */
		// expand_variables(tokens, shell);

		/* parse tokens into commands */
		// call parser to build command structures from tokens
		commands = parse_tokens(tokens, arena);
		if (!commands && tokens)                 // parse failed (syntax or alloc)
		{
<<<<<<< HEAD
			/* Tokenize the input; tokenize returns NULL on error */
			tokens = tokenize(input);
			if (!tokens)
			{
				/* Tokenization failed (unmatched quote or malloc fail) */
				/* We skip execution and continue; input must be freed */
				free(input);
				continue;
			}

			/* Debug: print tokens */
			// for (t_list *tmp = tokens; tmp; tmp = tmp->next)
			// 	printf("TOKEN: %s\n", (char *)tmp->content);
			temp = tokens;
			while (temp)
			{
				child_process(temp->content, env);
			}

			/* Free token list after processing */
			ft_lstclear(&tokens, free);
=======
			// parsing error already printed by parse_tokens()
			// continue loop; don't try to execute
			(void)tokens;
			continue;
>>>>>>> main
		}
		dbg_print_cmds(commands);                // show parsed commands

		/* execute commands (disabled until implemented) */
		// execute_command(shell, commands);

		dbg_print_exit_code(shell->exit_code);   // debug exit code

		/* avoid unused variable warnings while features are stubbed */
		(void)tokens;
		(void)commands;
	}
}
