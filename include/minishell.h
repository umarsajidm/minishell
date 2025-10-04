/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achowdhu <achowdhu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/29 17:35:10 by achowdhu          #+#    #+#             */
/*   Updated: 2025/09/30 15:13:02 by achowdhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

/* ===========================
**        Libraries
** =========================== */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

/* ===========================
**        Structures
** =========================== */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env;
	int		exit_code;
	bool	running;
}	t_shell;

/* ===========================
**        Main
** =========================== */
int		main(int argc, char **argv, char **envp);

/* ===========================
**        REPL / Loop
** =========================== */
void	repl_loop(t_shell *shell);
char	*read_input(void);

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

/* ===========================
**        Utils
** =========================== */
void	shell_error(const char *msg);

/* ===========================
**        Tokenize / Lexer
** =========================== */
t_list	*tokenize(char *input);
int		skip_spaces(char *s, int i);
int		is_operator_char(char c);
char	*dup_word(const char *str, int start, int end);
int		handle_operator(char *s, int i, t_list **tokens);
int		handle_quote(char *s, int i, t_list **tokens);
int		handle_word(char *s, int i, t_list **tokens);

#endif
