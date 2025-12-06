#ifndef EXECUTION_H
#define EXECUTION_H

#include "minishell.h"

#define COMMAND_NOT_FOUND 127
#define PERMISSION_DENIED 126
#define EXECUTION_SUCESS 0
#define GENERAL_ERROR 1
#define ENV_PATH_COULDNT_BE_FOUND 2
#define ENVIRONMENT_COPY_FAILED 3
#define FORK_FAILED 4

//pipeline_exit_utilities
void set_the_code_and_exit(t_shell *shell, int type, char *str, char **array);
void exit_after_execve(t_shell *shell, char *str, char **array);

//execution.c
// UPDATED: Now includes path_to_exec
int child_process(t_cmd *cmd, t_shell *shell, t_fd *fd, char **env);

//envp.c
char **envp_arr(t_shell *shell);

//utils
void    strerrornexit(void);
void    freearray(char **arr);
void    freestrnarrexit(char **arr, char *str, int i);
void    freeerror(char **arr);
void    freeall(char **arr, char *str, char *cmd);
void    commandnotfound(char **arr);

//shell.c
int init_shell(t_shell *shell, char **envp, t_arena **arena);

//pipeline.c
void main_pipeline(t_cmd *command, t_shell *shell);
void close_fd(t_fd *fd);
// UPDATED: Now includes path_to_exec
int fds_manipulation_and_execution(t_cmd *cmd, t_shell *shell, t_fd *fd, char **arr, char *path_to_exec);
void cleanup_pipeline(t_shell *shell, char **envp, pid_t last_pid);
// void pipeline(t_cmd *command, t_shell *shell);

// UPDATED: Now includes path_to_exec
int execution(t_cmd *cmd, t_shell *shell, char **env, char *path_to_exec);

//pipeline_utils.c
void applying_redir(t_redir *r, int *in_fd, int *out_fd);
void waitstatus(pid_t pid,  t_shell *shell);


//builtin
int is_parent_level_builtin(t_cmd *cmd);


#endif
