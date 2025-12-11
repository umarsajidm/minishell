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

        ft_dprintf(2, "REDIR: %s -> %s\n", type_str, target);
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
    ft_dprintf(2, "\n-------- DEBUG TOKENS --------\n");
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

            ft_dprintf(2, "[%d] \"%s\" (%s)\n", i, tok->token, type_str);
        }
        else
            ft_dprintf(2, "[%d] (null)\n", i);

        i++;
        tokens = tokens->next;
    }
    ft_dprintf(2, "------------------------------\n\n");
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
    ft_dprintf(2, "------- DEBUG COMMANDS -------\n");
    i = 0;
    while (cmd)
    {
        ft_dprintf(2, "CMD[%d]:", i);
        if (!cmd->argv || !cmd->argv[0])
            ft_dprintf(2, " (empty)\n");
        else
        {
            j = 0;
            while (cmd->argv[j])
            {
                ft_dprintf(2, " '%s'", cmd->argv[j]);
                j++;
            }
            ft_dprintf(2, "\n");
        }

        print_redir(cmd->redirs);
        i++;
        cmd = cmd->next;
    }
    ft_dprintf(2, "------------------------------\n\n");
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
                ft_dprintf(2, "expanded argv[%d]: %s\n", i, cmd->argv[i]);
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
    ft_dprintf(2, "\n------ DEBUG EXIT CODE -------\n");
    ft_dprintf(2, "Exit code: %d\n", code);
    ft_dprintf(2, "------------------------------\n\n");
}
