#include "minishell.h"

void set_the_code_and_exit(int type, char *str, char **array)
{
	// t_shell *shell;
	t_fd	fd;

	if (type == PERMISSION_DENIED)
		perror("permission denied");
	else if (type == COMMAND_NOT_FOUND)
		perror("command not found");
	else if (type == ENV_PATH_COULDNT_BE_FOUND)
		perror("retrievinig path from env failed");
	else if (type == ENVIRONMENT_COPY_FAILED)
		perror("copying env for child process failed");
	else if (type == FORK_FAILED)
		perror("forking failed");
	close_fd(&fd);
	freearray(array);
	if (str != NULL)
		free(str);
	exit(type);
}

// void set_exit_and_free_array(char **array)
// {

// }


void exit_after_execve(char *str, char **array)
{
	if (errno == ENOENT)
		set_the_code_and_exit(COMMAND_NOT_FOUND, str, array);
	else if (errno == ENOENT)
		set_the_code_and_exit(PERMISSION_DENIED, str, array);
	else
		set_the_code_and_exit(GENERAL_ERROR, str, array);
}


