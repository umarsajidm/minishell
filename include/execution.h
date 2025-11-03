#ifndef EXECUTION_H
#define EXECUTION_H

void	child_process(t_cmd *parsed_cmd, char **env);

//utils
void	strerrornexit(void);
void	freearray(char **arr);
void	freestrnarrexit(char **arr, char *str, int i);
void	freeerror(char **arr);
void	freeall(char **arr, char *str, char *cmd);
void	commandnotfound(char **arr);

//might delete this later
char **copy_envp(char **envp);

#endif