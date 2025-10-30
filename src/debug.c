/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 15:23:10 by achowdhu          #+#    #+#             */
/*   Updated: 2025/10/30 15:45:34 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* 
 * Print a single redirection list
 * - Prints type (<, >, >>, <<) and target
 */
static void	print_redir(t_redir *r)
{
	while (r)
	{
		ft_printf("    REDIR: %s -> %s\n", 
			(r->type == R_INPUT) ? "<" : 
			(r->type == R_OUTPUT) ? ">" : 
			(r->type == R_APPEND) ? ">>" : "<<",
			r->target ? r->target : "(null)");     // print type and target
		r = r->next;                               // move to next redirection
	}
}

/* 
 * Print all tokens for debugging
 * - Shows token index, string content, and type
 * - Adds blank lines before and after section
 */
void	dbg_print_tokens(t_list *tokens)
{
	int		i;                                      // token index
	t_token *tok;                                  // current token

	ft_printf("\n---- DEBUG TOKENS ----\n");       // blank line before
	i = 0;
	while (tokens)
	{
		tok = tokens->content;                     // cast content to token
		if (tok && tok->str)
			ft_printf("[%d] \"%s\" (%s)\n", i, tok->str, 
				(tok->type == T_WORD) ? "WORD" :
				(tok->type == T_QUOTE) ? "QUOTE" : "OPERATOR"); // print string + type
		else
			ft_printf("[%d] (null)\n", i);         // null token

		i++;                                        // next index
		tokens = tokens->next;                     // move to next token
	}
	ft_printf("----------------------\n\n");        // blank line after
}

/* 
 * Print all commands for debugging
 * - Shows command index, argv contents, and redirections
 * - Adds blank lines before and after section
 */
void	dbg_print_cmds(t_cmd *cmd)
{
	int	i;                                          // command index
	int	j;                                          // argv index

	ft_printf("\n---- DEBUG COMMANDS ----\n");       // blank line before
	i = 0;
	while (cmd)
	{
		ft_printf("CMD[%d]:", i);                   // print command index
		if (!cmd->argv || !cmd->argv[0])
			ft_printf(" (empty)\n");               // empty argv
		else
		{
			j = 0;
			while (cmd->argv[j])
			{
				ft_printf(" '%s'", cmd->argv[j]); // print argv element
				j++;                               // next argv
			}
			ft_printf("\n");                       // newline after argv
		}

		print_redir(cmd->redirs);                    // print redirections
		i++;                                         // next command
		cmd = cmd->next;                             // move to next
	}
	ft_printf("------------------------\n\n");       // blank line after
}

/* 
 * Print last exit code for debugging
 * - Adds blank lines before and after section
 */
void	dbg_print_exit_code(int code)
{
	ft_printf("\n---- DEBUG EXIT CODE ----\n");     // blank line before
	ft_printf("Exit code: %d\n", code);             // print exit code
	ft_printf("-------------------------\n\n");    // blank line after
}
