#ifndef MINISHELL_H
# define MINISHELL_H

/* ===========================
**        Standard Libraries
** =========================== */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/stat.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>

/* ===========================
**        Libft (types like t_list)
** =========================== */
# include "libft.h"

/* ===========================
**        Minishell Types
** =========================== */
# include "minishell_types.h"

/* ===========================
**        Arena core functions
** =========================== */
void	*arena_alloc(t_arena **arena, size_t size);
void	*arena_realloc(t_arena **arena, void *old_ptr, size_t old_size,
			size_t new_size);
t_arena	*init_arena(size_t size);
t_arena	*new_bigger_arena(t_arena *current_arena, size_t size);
char	*arena_strdup(t_arena **arena, const char *s);
void	free_arena(t_arena **arena);
void	arena_clear(t_arena **arena);

/* ===========================
**            Main
** =========================== */
int		main(int argc, char **argv, char **envp);

/* ===========================
**            REPL
** =========================== */
void	repl_loop(t_shell *shell, t_arena **arena);
char	*read_input(t_arena **arena);
char	*read_heredoc(t_arena **arena, const char *delimiter);
char	*handle_heredoc(t_cmd *cmd, t_arena **arena, const char *delimiter);

/* ===========================
**        Signals
** =========================== */
void	setup_signals(void);
void	handle_sigint(int sig);
void	handle_sigquit(int sig);

/* ===========================
**        Environment
** =========================== */
t_env	*init_env(char **envp);
void	free_env(t_env *env);
char	*get_env_value(t_env *env, const char *key);
int		set_env_var(t_shell *shell, const char *key, const char *value);
void	dbg_print_env(t_env *env);

/* ===========================
**         Tokenizer
** =========================== */
t_list	*tokenize(char *input, t_arena **arena);
int		skip_spaces(char *s, int i);
int		is_operator_char(char c);
char	*dup_word(t_arena **arena, const char *str, int start, int end);
int		handle_operator(char *s, int i, t_list **tokens, t_arena **arena);
int		handle_quote(char *s, int i, t_list **tokens, t_arena **arena);
int		handle_word(char *s, int i, t_list **tokens, t_arena **arena);
void	free_tokens(t_list **tokens);

/* ===========================
**          Parser
** =========================== */
t_cmd	*parse_tokens(t_list *tokens, t_shell *shell, t_arena **arena);
t_cmd	*create_cmd_node(t_arena **arena);
int		add_word_to_argv(t_cmd *cmd, const char *word, t_arena **arena);
int		add_redirection(t_cmd *cmd, t_redir_type type, const char *target,
			t_arena **arena);
int		handle_word_token(t_cmd **cur, t_cmd **head, t_token *tok,
			t_shell *shell, t_arena **arena);
int		handle_pipe_token(t_token *tok, t_cmd **cur);
int		handle_operator_token(t_list **tokens_ref, t_cmd **cur,
			t_cmd **head, t_arena **arena);
int		handle_redir_token(t_list **tokens_ref, t_cmd **cur,
			t_cmd **head, t_arena **arena);
int		is_pipe_token(const char *tok);
int		is_redir_token(const char *tok);
t_redir_type	get_redir_type(const char *tok);

/* ===========================
**         Expansion
** =========================== */
char	*expand_variable(const char *str, size_t *i,
	t_shell *shell, t_arena **arena);
char	*expand_string(const char *str, t_shell *shell, t_arena **arena);
int		expand_command_argv(t_cmd *cmd, t_shell *shell, t_arena **arena);

/* ===========================
**        Executor
** =========================== */
void	execute_command(t_shell *shell, t_cmd *cmds);
int		exec_single_cmd(t_shell *shell, t_cmd *cmd);
char	*find_executable(char *cmd, t_env *env);
int		exec_builtins(t_shell *shell, t_cmd *cmd);

/* ===========================
**        Builtins
** =========================== */
int		builtin_echo(t_list *args);
int		builtin_cd(t_shell *shell, t_list *args);
int		builtin_pwd(void);
int		builtin_export(t_shell *shell, t_list *args);
int		builtin_unset(t_shell *shell, t_list *args);
int		builtin_env(t_shell *shell);
int		builtin_exit(t_shell *shell, t_list *args);

/* ===========================
**        Utils
** =========================== */
t_cmd	*parse_error(const char *msg, const char *tok);
int		count_argv(char **argv);
int		iterate_key(const char *str);

/* ===========================
**        Debug
** =========================== */
void	dbg_print_tokens(t_list *tokens);
void	dbg_print_cmds(t_cmd *cmds);
void	dbg_print_exit_code(int exit_code);
void	dbg_print_expanded_argv(t_cmd *cmd);

#endif /* MINISHELL_H */
