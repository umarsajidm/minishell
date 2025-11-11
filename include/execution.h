#ifndef EXECUTION_H
#define EXECUTION_H

#include "minishell.h"
//execution.c
void	child_process(t_cmd *parsed_cmd, t_shell *shell);

//envp.c
char **envp_arr(t_shell *shell);

//utils
void	strerrornexit(void);
void	freearray(char **arr);
void	freestrnarrexit(char **arr, char *str, int i);
void	freeerror(char **arr);
void	freeall(char **arr, char *str, char *cmd);
void	commandnotfound(char **arr);

//shell.c
int init_shell(t_shell *shell, char **envp, t_arena **arena);

//pipeline.c
void execution_pipeline(t_cmd *command);
void pipeline(t_cmd	*command, t_shell *shell);

#endif