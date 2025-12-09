#include "minishell.h"

// /*
//  * Shared Helper: Processes a single line of input.
//  * - Used by BOTH interactive and non-interactive modes.
//  * - Handles tokenizing, parsing, expansion, and execution.
//  */
static void process_line(t_shell *shell, t_arena **arena, char *input)
{
    t_list  *tokens;
    t_cmd   *commands;
    int     res;

    /* if the line is empty or only whitespace, skip processing */
    if (is_blank_line(input))
        return ;

    tokens = tokenize(input, arena);         // tokenize input using arena

    /* tokenize() returns NULL on error (e.g. unmatched quote) */
    if (!tokens)
    {
        /* tokenize already printed the syntax error */
        shell->exit_code = 2;
        return ;
    }

    // dbg_print_tokens(tokens);              // debug tokens

    /* parse tokens into commands */
    commands = parse_tokens(tokens, shell, arena);
    if (!commands)                 // parse failed (syntax or alloc)
    {
        /* parse_tokens should print the error */
        shell->exit_code = 2;
        return ;
    }
    // dbg_print_cmds(commands);              // show parsed commands

    /* variable expansion */
    res = expand_command_argv(commands, shell, arena);
    if (res == 0)                            // allocation failure during expansion
    {
        ft_printf("minishell: parse error: alloc fail\n"); // formatted error
        if (shell->exec->fd != NULL)
            free(shell->exec->fd);
        shell->exit_code = 1;
        return ;
    }
    // dbg_print_expanded_argv(commands);

    if (commands->argv != NULL)
        main_pipeline(shell, commands);
}

// // /*
// //  * Interactive REPL loop
// //  * - Uses readline for history and editing
// //  * - Handles interactive signals
// //  */
// // void    repl_loop(t_shell *shell, t_arena **arena)
// // {
// //     char    *input;

// //     while (shell->running)
// //     {
// //         input = read_input(arena);           // read user input into arena
// //         if (!input)
// //         {
// //             ft_printf("exit\n");
// //             break ;
// //         }

// //         if (*input)
// //             add_history(input);

// //         process_line(shell, arena, input);
// //         arena_clear(arena);                  // wipe memory for next line
// //     }
// // }

// // /*
// //  * Non-Interactive Loop (Pipes)
// //  * - Uses get_next_line (no prompt, no history)
// //  * - Handles input from pipes (echo "ls" | ./minishell)
// //  */
// // void    non_interactive_loop(t_shell *shell, t_arena **arena)
// // {
// //     char    *line;
// //     char    *input;

// //     while (shell->running)
// //     {
// //         line = get_next_line(STDIN_FILENO);
// //         if (!line)
// //             break ;

// //         // Copy to arena to match your memory model
// //         input = arena_strdup(arena, line);
// //         free(line);                          // free GNL buffer

// //         // Remove trailing newline from GNL
// //         if (input)
// //         {
// //             size_t len = ft_strlen(input);
// //             if (len > 0 && input[len - 1] == '\n')
// //                 input[len - 1] = '\0';
// //         }

// //         process_line(shell, arena, input);
// //         arena_clear(arena);
// //     }
// // }

// /*
//  * Interactive REPL loop
//  * - Uses readline for history and editing
//  * - Handles interactive signals
//  */
// void    repl_loop(t_shell *shell, t_arena **arena)
// {
//     char    *input;

//     while (shell->running)
//     {
//         input = read_input(arena);           // read user input into arena
//         if (!input)
//         {
//             // TESTER FIX: Do not print "exit" here.
//             // The shell should exit silently on CTRL+D during tests, 
//             // or only print it if you really want to (but testers hate it).
//             // ft_printf("exit\n"); 
//             break ;
//         }
//         process_line(shell, arena, input);
//         arena_clear(arena);                  // wipe memory for next line
//     }
// }

// /*
//  * Non-Interactive Loop (Pipes)
//  * - Uses get_next_line (no prompt, no history)
//  * - Handles input from pipes (echo "ls" | ./minishell)
//  */
// void    non_interactive_loop(t_shell *shell, t_arena **arena)
// {
//     char    *line;
//     char    *input;

  
//         line = get_next_line(STDIN_FILENO);
//         if (!line)
//             return; // End of input/pipe

//         // 1. Trim the newline (The tester suggested ft_strtrim, but manual is faster/safer here)
//         // We do this BEFORE copying to arena to save space, or inside the copy.
//         size_t len = ft_strlen(line);
//         if (len > 0 && line[len - 1] == '\n')
//             line[len - 1] = '\0';

//         // 2. Copy to your arena
//         input = arena_strdup(arena, line);
        
//         // 3. Free the GNL buffer immediately
//         free(line);

//         // 4. Process
//         if (input) // Ensure allocation worked
//             process_line(shell, arena, input);
            
//         arena_clear(arena);
// }

/*
 * Interactive REPL loop
 * - Uses readline (via read_input)
 * - Handles interactive signals
 */
void    repl_loop(t_shell *shell, t_arena **arena)
{
    char    *input;

    while (shell->running)
    {
        input = read_input(arena);           // read user input into arena
        if (!input)
        {
            /* * TESTER FIX: Commented out "exit" message.
             * Shell exits silently on CTRL+D as required by strict testers.
             */
            // ft_printf("exit\n");
            break ;
        }

        /* * Optional: Add history here if read_input doesn't do it. 
         * Standard readline usage requires adding non-empty lines to history.
         */
        if (*input)
            add_history(input);

        process_line(shell, arena, input);
        arena_clear(arena);                  // wipe memory for next line
    }
}

/*
 * Non-Interactive Loop (Pipes)
 * - Uses get_next_line (no prompt, no history)
 * - Loops until GNL returns NULL
 */
/*
 * Non-Interactive Loop (Pipes)
 * - Uses get_next_line (no prompt, no history)
 * - Loops until GNL returns NULL to handle scripts/multiple commands
 */
void    non_interactive_loop(t_shell *shell, t_arena **arena)
{
    char    *line;
    char    *input;

    while (shell->running)
    {
        line = get_next_line(STDIN_FILENO);
        if (!line)
            return; // End of input/pipe (CTRL+D or End of Script)

        /* Trim the newline manually. */
        size_t len = ft_strlen(line);
        if (len > 0 && line[len - 1] == '\n')
            line[len - 1] = '\0';

        /* Copy to your arena */
        input = arena_strdup(arena, line);
        
        /* Free the GNL buffer immediately */
        free(line);

        /* Process the line */
        if (input) 
            process_line(shell, arena, input);
            
        arena_clear(arena);
    }
}