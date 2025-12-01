#include "minishell.h"

/* * Print a single redirection list
 * - Prints type (<, >, >>, <<) and target
 * - No indentation
 */
static void print_redir(t_redir *r)
{
    char    *type_str;
    char    *target;

    while (r)
    {
        if (r->type == R_INPUT)
            type_str = "<";
        else if (r->type == R_OUTPUT)
            type_str = ">";
        else if (r->type == R_APPEND)
            type_str = ">>";
        else
            type_str = "<<";

        if (r->target)
            target = r->target;
        else
            target = "(null)";

        ft_printf("REDIR: %s -> %s\n", type_str, target);
        r = r->next;
    }
}

/* * Print all tokens for debugging
 * - Shows token index, string content, and type
 * - Aligned header/footer width (30 chars)
 */
void    dbg_print_tokens(t_list *tokens)
{
    int     i;
    t_token *tok;
    char    *type_str;

    // Header width: 8 + 1 + 12 + 1 + 8 = 30 chars
    ft_printf("\n-------- DEBUG TOKENS --------\n");
    i = 0;
    while (tokens)
    {
        tok = tokens->content;
        if (tok && tok->token)
        {
            if (tok->type == T_WORD)
                type_str = "WORD";
            else if (tok->type == T_QUOTE)
                type_str = "QUOTE";
            else
                type_str = "OPERATOR";

            ft_printf("[%d] \"%s\" (%s)\n", i, tok->token, type_str);
        }
        else
            ft_printf("[%d] (null)\n", i);

        i++;
        tokens = tokens->next;
    }
    ft_printf("------------------------------\n\n");
}

/* * Print all commands for debugging
 * - Shows command index, argv contents, and redirections
 * - Aligned header/footer width (30 chars)
 */
void    dbg_print_cmds(t_cmd *cmd)
{
    int i;
    int j;

    // Header width: 7 + 1 + 14 + 1 + 7 = 30 chars
    ft_printf("------- DEBUG COMMANDS -------\n");
    i = 0;
    while (cmd)
    {
        ft_printf("CMD[%d]:", i);
        if (!cmd->argv || !cmd->argv[0])
            ft_printf(" (empty)\n");
        else
        {
            j = 0;
            while (cmd->argv[j])
            {
                ft_printf(" '%s'", cmd->argv[j]);
                j++;
            }
            ft_printf("\n");
        }

        print_redir(cmd->redirs);
        i++;
        cmd = cmd->next;
    }
    ft_printf("------------------------------\n\n");
}

/* * Print expanded argv of commands
 * - Norminette safe
 * - Shows variable expansions
 */
void    dbg_print_expanded_argv(t_cmd *cmd)
{
    int i;

    while (cmd)
    {
        i = 0;
        if (cmd->argv)
        {
            while (cmd->argv[i])
            {
                ft_printf("expanded argv[%d]: %s\n", i, cmd->argv[i]);
                i++;
            }
        }
        cmd = cmd->next;
    }
}

/* * Print last exit code for debugging
 * - Aligned header/footer width (30 chars)
 */
void    dbg_print_exit_code(int code)
{
    // Header width: 6 + 1 + 15 + 1 + 7 = 30 chars
    ft_printf("\n------ DEBUG EXIT CODE -------\n");
    ft_printf("Exit code: %d\n", code);
    ft_printf("------------------------------\n\n");
}
